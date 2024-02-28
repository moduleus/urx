#include <memory>
#include <unordered_map>
#include <utility>

#include <urx/enums.h>
#include <urx/utils/io/enums.h>

namespace urx::utils::io::enums {

namespace {
std::unordered_map<urx::ProbeType, std::string> probe_to_string{
    {urx::ProbeType::LINEAR, "LINEAR"}, {urx::ProbeType::CURVILINEAR, "CURVILINEAR"},
    {urx::ProbeType::RCA, "RCA"},       {urx::ProbeType::MATRIX, "MATRIX"},
    {urx::ProbeType::SPARSE, "SPARSE"}, {urx::ProbeType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::ProbeType> string_to_probe = invertMap(probe_to_string);

std::unordered_map<urx::WaveType, std::string> wave_to_string{
    {urx::WaveType::CONVERGING_WAVE, "CONVERGING_WAVE"},
    {urx::WaveType::DIVERGING_WAVE, "DIVERGING_WAVE"},
    {urx::WaveType::PLANE_WAVE, "PLANE_WAVE"},
    {urx::WaveType::CYLINDRICAL_WAVE, "CYLINDRICAL_WAVE"},
    {urx::WaveType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::WaveType> string_to_wave = invertMap(wave_to_string);

std::unordered_map<urx::SamplingType, std::string> sampling_to_string{
    {urx::SamplingType::RF, "RF"},
    {urx::SamplingType::IQ, "IQ"},
    {urx::SamplingType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::SamplingType> string_to_sampling =
    invertMap(sampling_to_string);

std::unordered_map<urx::DataType, std::string> data_to_string{
    {urx::DataType::INT16, "INT16"},
    {urx::DataType::INT32, "INT32"},
    {urx::DataType::FLOAT, "FLOAT"},
    {urx::DataType::DOUBLE, "DOUBLE"},
    {urx::DataType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::DataType> string_to_data = invertMap(data_to_string);

}  // namespace

template <>
urx::ProbeType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_probe);
}
template <>
urx::WaveType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_wave);
}
template <>
urx::SamplingType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_sampling);
}
template <>
urx::DataType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_data);
}

template <>
std::string enumToString(urx::ProbeType enumeration) {
  return enumToStringImpl(enumeration, probe_to_string);
}
template <>
std::string enumToString(urx::WaveType enumeration) {
  return enumToStringImpl(enumeration, wave_to_string);
}
template <>
std::string enumToString(urx::SamplingType enumeration) {
  return enumToStringImpl(enumeration, sampling_to_string);
}
template <>
std::string enumToString(urx::DataType enumeration) {
  return enumToStringImpl(enumeration, data_to_string);
}

}  // namespace urx::utils::io::enums
