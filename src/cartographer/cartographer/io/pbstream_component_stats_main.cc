/*
 * Diagnostic-only pbstream component analyzer for Cartographer.
 *
 * Reads an existing .pbstream file without modifying it and reports, for each
 * serialized record type:
 *   - record count
 *   - uncompressed protobuf bytes
 *   - compressed payload bytes exactly stored in pbstream
 *   - stream bytes including each 8-byte record length prefix
 *   - compression ratio
 *   - diagnostic gunzip / protobuf parse time
 *
 * This executable is intentionally separate from SerializeState()/LoadState()
 * timing measurements so that component-size inspection does not perturb the
 * baseline handover latency measurement path.
 */

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "cartographer/common/port.h"
#include "cartographer/mapping/proto/serialization.pb.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_string(pbstream_filename, "", "Input .pbstream filename.");
DEFINE_string(summary_csv, "", "Output component-summary CSV filename.");
DEFINE_string(records_csv, "", "Output per-record CSV filename.");

namespace cartographer {
namespace io {
namespace {

constexpr uint64_t kMagic = 0x7b1d1f7b5bf501dbULL;
constexpr uint64_t kMagicBytes = 8;
constexpr uint64_t kRecordLengthPrefixBytes = 8;

using SteadyClock = std::chrono::steady_clock;

struct ComponentStats {
  uint64_t record_count = 0;
  uint64_t uncompressed_bytes = 0;
  uint64_t compressed_payload_bytes = 0;
  uint64_t stream_bytes = 0;
  double gunzip_ms = 0.;
  double parse_ms = 0.;
};

struct RecordStats {
  uint64_t record_index = 0;
  std::string component;
  uint64_t uncompressed_bytes = 0;
  uint64_t compressed_payload_bytes = 0;
  uint64_t stream_bytes = 0;
  double gunzip_ms = 0.;
  double parse_ms = 0.;
};

double Milliseconds(const SteadyClock::duration duration) {
  return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
             duration)
      .count();
}

bool ReadSizeAsLittleEndian(std::istream* in, uint64_t* size) {
  *size = 0;
  for (int i = 0; i != 8; ++i) {
    const int ch = in->get();
    if (ch == EOF) {
      return false;
    }
    *size |= static_cast<uint64_t>(static_cast<uint8_t>(ch)) << (8 * i);
  }
  return true;
}

std::string DataCaseName(
    const mapping::proto::SerializedData::DataCase data_case) {
  using SerializedData = mapping::proto::SerializedData;
  switch (data_case) {
    case SerializedData::kPoseGraph:
      return "pose_graph";
    case SerializedData::kAllTrajectoryBuilderOptions:
      return "all_trajectory_builder_options";
    case SerializedData::kSubmap:
      return "submap";
    case SerializedData::kNode:
      return "trajectory_node";
    case SerializedData::kTrajectoryData:
      return "trajectory_data";
    case SerializedData::kImuData:
      return "imu_data";
    case SerializedData::kOdometryData:
      return "odometry_data";
    case SerializedData::kFixedFramePoseData:
      return "fixed_frame_pose_data";
    case SerializedData::kLandmarkData:
      return "landmark_data";
    case SerializedData::DATA_NOT_SET:
      return "data_not_set";
  }
  return "unknown";
}

std::string CsvEscape(const std::string& value) {
  if (value.find_first_of(",\"\n\r") == std::string::npos) {
    return value;
  }
  std::string escaped = "\"";
  for (const char c : value) {
    if (c == '\"') escaped += '\"';
    escaped += c;
  }
  escaped += "\"";
  return escaped;
}

bool WriteRecordsCsv(const std::string& filename,
                     const std::vector<RecordStats>& records) {
  if (filename.empty()) return true;
  std::ofstream out(filename);
  if (!out.is_open()) return false;
  out << "record_index,component,uncompressed_bytes,compressed_payload_bytes,"
         "stream_bytes,compression_ratio,gunzip_ms,parse_ms\n";
  out << std::fixed << std::setprecision(6);
  for (const auto& record : records) {
    const double ratio = record.uncompressed_bytes == 0
                             ? 0.
                             : static_cast<double>(record.compressed_payload_bytes) /
                                   static_cast<double>(record.uncompressed_bytes);
    out << record.record_index << ',' << CsvEscape(record.component) << ','
        << record.uncompressed_bytes << ',' << record.compressed_payload_bytes
        << ',' << record.stream_bytes << ',' << ratio << ',' << record.gunzip_ms
        << ',' << record.parse_ms << '\n';
  }
  return out.good();
}

std::vector<std::string> OrderedComponents() {
  return {
      "format_magic",
      "serialization_header",
      "pose_graph",
      "all_trajectory_builder_options",
      "submap",
      "trajectory_node",
      "trajectory_data",
      "imu_data",
      "odometry_data",
      "fixed_frame_pose_data",
      "landmark_data",
      "data_not_set",
      "unknown",
  };
}

bool WriteSummaryCsv(const std::string& filename,
                     const std::map<std::string, ComponentStats>& stats,
                     const uint64_t file_bytes) {
  if (filename.empty()) return true;
  std::ofstream out(filename);
  if (!out.is_open()) return false;
  out << "component,record_count,uncompressed_bytes,compressed_payload_bytes,"
         "stream_bytes,percent_of_pbstream,compression_ratio,gunzip_ms,parse_ms\n";
  out << std::fixed << std::setprecision(6);

  ComponentStats total;
  for (const auto& component : OrderedComponents()) {
    const auto it = stats.find(component);
    if (it == stats.end()) continue;
    const auto& s = it->second;
    const double percent = file_bytes == 0
                               ? 0.
                               : 100. * static_cast<double>(s.stream_bytes) /
                                     static_cast<double>(file_bytes);
    const double ratio = s.uncompressed_bytes == 0
                             ? 0.
                             : static_cast<double>(s.compressed_payload_bytes) /
                                   static_cast<double>(s.uncompressed_bytes);
    out << CsvEscape(component) << ',' << s.record_count << ','
        << s.uncompressed_bytes << ',' << s.compressed_payload_bytes << ','
        << s.stream_bytes << ',' << percent << ',' << ratio << ','
        << s.gunzip_ms << ',' << s.parse_ms << '\n';

    total.record_count += s.record_count;
    total.uncompressed_bytes += s.uncompressed_bytes;
    total.compressed_payload_bytes += s.compressed_payload_bytes;
    total.stream_bytes += s.stream_bytes;
    total.gunzip_ms += s.gunzip_ms;
    total.parse_ms += s.parse_ms;
  }

  const double total_ratio = total.uncompressed_bytes == 0
                                 ? 0.
                                 : static_cast<double>(
                                       total.compressed_payload_bytes) /
                                       static_cast<double>(
                                           total.uncompressed_bytes);
  out << "TOTAL," << total.record_count << ',' << total.uncompressed_bytes
      << ',' << total.compressed_payload_bytes << ',' << total.stream_bytes
      << ",100.000000," << total_ratio << ',' << total.gunzip_ms << ','
      << total.parse_ms << '\n';
  return out.good();
}

int Run() {
  if (FLAGS_pbstream_filename.empty()) {
    std::cerr << "--pbstream_filename is required.\n";
    return 2;
  }

  std::ifstream in(FLAGS_pbstream_filename, std::ios::binary);
  if (!in.is_open()) {
    std::cerr << "Failed to open: " << FLAGS_pbstream_filename << '\n';
    return 3;
  }

  in.seekg(0, std::ios::end);
  const auto end_pos = in.tellg();
  if (end_pos < 0) {
    std::cerr << "Failed to determine file size.\n";
    return 4;
  }
  const uint64_t file_bytes = static_cast<uint64_t>(end_pos);
  in.seekg(0, std::ios::beg);

  uint64_t magic = 0;
  if (!ReadSizeAsLittleEndian(&in, &magic) || magic != kMagic) {
    std::cerr << "Invalid pbstream magic.\n";
    return 5;
  }

  std::map<std::string, ComponentStats> stats;
  stats["format_magic"].record_count = 1;
  stats["format_magic"].stream_bytes = kMagicBytes;

  std::vector<RecordStats> records;
  uint64_t record_index = 0;

  while (true) {
    const std::streampos prefix_pos = in.tellg();
    uint64_t compressed_size = 0;
    if (!ReadSizeAsLittleEndian(&in, &compressed_size)) {
      // Clean EOF exactly after the previous record is valid.
      in.clear();
      in.seekg(0, std::ios::end);
      if (prefix_pos == in.tellg()) break;
      std::cerr << "Truncated record-length prefix after record "
                << record_index << ".\n";
      return 6;
    }

    std::string compressed_data(compressed_size, '\0');
    if (compressed_size > 0 &&
        !in.read(&compressed_data.front(), compressed_size)) {
      std::cerr << "Truncated compressed payload at record " << record_index
                << ".\n";
      return 7;
    }

    const auto gunzip_begin = SteadyClock::now();
    std::string uncompressed_data;
    common::FastGunzipString(compressed_data, &uncompressed_data);
    const auto gunzip_end = SteadyClock::now();

    std::string component;
    const auto parse_begin = SteadyClock::now();
    bool parse_ok = false;
    if (record_index == 0) {
      mapping::proto::SerializationHeader header;
      parse_ok = header.ParseFromString(uncompressed_data);
      component = "serialization_header";
    } else {
      mapping::proto::SerializedData proto;
      parse_ok = proto.ParseFromString(uncompressed_data);
      if (parse_ok) component = DataCaseName(proto.data_case());
    }
    const auto parse_end = SteadyClock::now();

    if (!parse_ok) {
      std::cerr << "Failed to parse protobuf record " << record_index << ".\n";
      return 8;
    }

    RecordStats record;
    record.record_index = record_index;
    record.component = component;
    record.uncompressed_bytes = uncompressed_data.size();
    record.compressed_payload_bytes = compressed_size;
    record.stream_bytes = kRecordLengthPrefixBytes + compressed_size;
    record.gunzip_ms = Milliseconds(gunzip_end - gunzip_begin);
    record.parse_ms = Milliseconds(parse_end - parse_begin);
    records.push_back(record);

    auto& s = stats[component];
    ++s.record_count;
    s.uncompressed_bytes += record.uncompressed_bytes;
    s.compressed_payload_bytes += record.compressed_payload_bytes;
    s.stream_bytes += record.stream_bytes;
    s.gunzip_ms += record.gunzip_ms;
    s.parse_ms += record.parse_ms;

    ++record_index;
  }

  uint64_t accounted_stream_bytes = 0;
  for (const auto& item : stats) accounted_stream_bytes += item.second.stream_bytes;
  if (accounted_stream_bytes != file_bytes) {
    std::cerr << "Byte-accounting mismatch: file_bytes=" << file_bytes
              << " accounted_stream_bytes=" << accounted_stream_bytes << '\n';
    return 9;
  }

  if (!WriteRecordsCsv(FLAGS_records_csv, records)) {
    std::cerr << "Failed to write records CSV: " << FLAGS_records_csv << '\n';
    return 10;
  }
  if (!WriteSummaryCsv(FLAGS_summary_csv, stats, file_bytes)) {
    std::cerr << "Failed to write summary CSV: " << FLAGS_summary_csv << '\n';
    return 11;
  }

  std::cout << "[PBSTREAM_COMPONENT_DIAG] file=" << FLAGS_pbstream_filename
            << " file_bytes=" << file_bytes
            << " records=" << record_index
            << " accounted_stream_bytes=" << accounted_stream_bytes << '\n';
  std::cout << std::fixed << std::setprecision(3);
  for (const auto& component : OrderedComponents()) {
    const auto it = stats.find(component);
    if (it == stats.end()) continue;
    const auto& s = it->second;
    const double percent = file_bytes == 0
                               ? 0.
                               : 100. * static_cast<double>(s.stream_bytes) /
                                     static_cast<double>(file_bytes);
    std::cout << "[PBSTREAM_COMPONENT] component=" << component
              << " count=" << s.record_count
              << " raw_bytes=" << s.uncompressed_bytes
              << " compressed_bytes=" << s.compressed_payload_bytes
              << " stream_bytes=" << s.stream_bytes
              << " percent=" << percent << '\n';
  }
  return 0;
}

}  // namespace
}  // namespace io
}  // namespace cartographer

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);
  return cartographer::io::Run();
}
