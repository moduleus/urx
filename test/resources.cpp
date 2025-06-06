#include "resources.h"

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <utility>

namespace test {

const std::string& getDataTestPath() {
  static std::string data_test;
  if (!data_test.empty()) {
    return data_test;
  }

#ifdef _WIN32
  char* env_test_data_path = nullptr;
  size_t len = 0;
  if (_dupenv_s(&env_test_data_path, &len, "TEST_DATA_PATH") == 0 && env_test_data_path) {
    data_test = env_test_data_path;
    free(env_test_data_path);
    return data_test;
  }
#else
  if (const char* env_test_data_path = std::getenv("TEST_DATA_PATH")) {
    data_test = env_test_data_path;
    return data_test;
  }
#endif

  const std::string data_test_hardcoded = TEST_DATA_PATH;
  if (std::filesystem::is_directory(data_test_hardcoded)) {
    data_test = data_test_hardcoded;
    return data_test;
  }

  throw std::runtime_error("Failed to found test/data path.");
}

}  // namespace test