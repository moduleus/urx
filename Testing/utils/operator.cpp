#include <uff/rca_array.h>
#include <uff/transmit_setup.h>
#include <uff/wave.h>
#include <catch2/catch.hpp>
#include <cstddef>
#include <memory>

#include "point.h"
#include "types.h"
#include "uff/uff.h"

namespace uff::test {

TEST_CASE("Wave::operator==", "[operator]") {
  auto w_1 = std::make_shared<Wave>();
  auto w_2 = std::make_shared<Wave>();

  REQUIRE(*w_1 == *w_2);

  REQUIRE(w_1 != w_2);
}

TEST_CASE("TransmitWave::operator==", "[operator]") {
  auto rca =
      std::make_shared<RcaArray>(Point2D<uint32_t>{64u, 64u}, Point2D<MetadataType>{1e-4, 75e-4});

  auto w_1 = std::make_shared<Wave>();
  auto w_2 = std::make_shared<Wave>();
  TransmitSetup ts_1(rca, w_1);
  const TransmitSetup ts_2(rca, w_1);

  REQUIRE(w_1 != w_2);
  REQUIRE(*w_1 == *w_2);
  REQUIRE(ts_1 == ts_2);

  ts_1.wave().lock()->setWaveType(WaveType::CYLINDRICAL_WAVE);
  REQUIRE(ts_1 == ts_2);

  ts_1.setWave(w_2);
  REQUIRE(ts_1 != ts_2);

  w_2->setWaveType(WaveType::CYLINDRICAL_WAVE);
  REQUIRE(ts_1 == ts_2);
}

}  // namespace uff::test
