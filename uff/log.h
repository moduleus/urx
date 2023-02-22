#pragma once

#ifdef __ANDROID__
#include <android/log.h>
#include <sstream>

namespace uff {
namespace {
std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to) {
  std::string retval = str;
  size_t start_pos = 0;
  while ((start_pos = retval.find(from, start_pos)) != std::string::npos) {
    retval.replace(start_pos, from.length(), to);
  }
  return retval;
}
template <typename T>
std::string GetTypeName() {
  // Android / clang only.
  std::string prefix = "[T = ";
  std::string suffix = "]";
  std::string function = __PRETTY_FUNCTION__;

  // Get Namespace::class
  const auto start = function.find(prefix) + prefix.size();
  const auto end = function.find(suffix);
  const auto size = end - start;

  std::string retval = function.substr(start, size);
  return ReplaceAll(retval, "::", ".");
}
}  // namespace

class MyStream {
 private:
  std::stringstream m_ss;
  int m_logLevel;
  std::string m_className;

 public:
  MyStream(int Xi_logLevel, const std::string& nameSpace) {
    m_logLevel = Xi_logLevel;
    m_className = nameSpace;
  };
  ~MyStream() {
    __android_log_print(m_logLevel, ("com.moduleus." + m_className).c_str(), "%s",
                        m_ss.str().c_str());
  }

  template <typename T>
  MyStream& operator<<(T const& Xi_val) {
    m_ss << Xi_val;
    return *this;
  }
};
}  // namespace uff

#define LOG_THIS(LOG_LEVEL)    \
  uff::MyStream(               \
      ANDROID_LOG_##LOG_LEVEL, \
      uff::GetTypeName<        \
          std::remove_reference_t<std::remove_pointer_t<std::remove_cv_t<decltype(this)>>>>())
#define LOG_NO_THIS(LOG_LEVEL)                                                                    \
  uff::MyStream(                                                                                  \
      ANDROID_LOG_##LOG_LEVEL,                                                                    \
      uff::ReplaceAll(                                                                            \
          std::string{__PRETTY_FUNCTION__}                                                        \
              .substr(0, std::string{__PRETTY_FUNCTION__}.find(__FUNCTION__ + std::string{"("}) + \
                             sizeof(__FUNCTION__) - 1)                                            \
              .substr(std::string{__PRETTY_FUNCTION__}.find(" ") + 1),                            \
          "::", "."))
#else
#include <iostream>

using LogPriority = enum { LOG_INFO = 0, LOG_WARN, LOG_ERROR };

#define LOG_THIS(LOG_LEVEL) (LOG_##LOG_LEVEL == LOG_INFO ? std::cout : std::cerr)
#define LOG_NO_THIS(LOG_LEVEL) (LOG_##LOG_LEVEL == LOG_INFO ? std::cout : std::cerr)
#endif
