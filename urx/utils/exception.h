#pragma once

#include <stdexcept>
#include <string>

namespace urx::utils {

class ReadFileException : public std::runtime_error {
 public:
  explicit ReadFileException(const std::string& message) : std::runtime_error(message) {}
  explicit ReadFileException(const char* message) : std::runtime_error(message) {}
};

class WriteFileException : public std::runtime_error {
 public:
  explicit WriteFileException(const std::string& message) : std::runtime_error(message) {}
  explicit WriteFileException(const char* message) : std::runtime_error(message) {}
};

class InvalidVersionException : public std::runtime_error {
 public:
  explicit InvalidVersionException(const std::string& message) : std::runtime_error(message) {}
  explicit InvalidVersionException(const char* message) : std::runtime_error(message) {}
};

}  // namespace urx::utils