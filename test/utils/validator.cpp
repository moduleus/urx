#include <algorithm>
#include <memory>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/probe.h>
#include <urx/utils/test/dataset_gen.h>
#include <urx/utils/validator.h>

namespace urx::utils::validator::test {

TEST_CASE("Test Dataset check", "[dataset][helper]") {
  std::shared_ptr<Dataset> dataset = std::make_shared<Dataset>();
  dataset->acquisition.probes = {std::make_shared<Probe>(), std::make_shared<Probe>()};

  ValidatorReport validator;
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 10);

  dataset = utils::test::generateFakeDataset<Dataset>();
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 102);

  dataset = utils::test::generateWrongDataset<Dataset>();
  validator.check(*dataset);

  REQUIRE(validator.getOutput().size() == 185);
}

}  // namespace urx::utils::validator::test
