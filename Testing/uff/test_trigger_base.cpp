#include <uff/trigger_base.h>
#include <catch2/catch.hpp>
#include <optional>

#include "trigger_in.h"
#include "trigger_location.h"
#include "trigger_out.h"

namespace uff::test {

TEST_CASE("TriggerBase::operator==", "[test_trigger_base]") {
  TriggerBase tb_1;
  REQUIRE(tb_1.triggerIn() == std::nullopt);
  REQUIRE(tb_1.triggerOut() == std::nullopt);

  TriggerBase tb_2 = TriggerBase(std::nullopt, std::nullopt);
  REQUIRE(tb_1 == tb_2);

  tb_2 = TriggerBase(TriggerIn(), std::nullopt);
  REQUIRE(tb_1 != tb_2);
  tb_2 = TriggerBase(std::nullopt, TriggerOut());
  REQUIRE(tb_1 != tb_2);
  tb_2 = TriggerBase(TriggerIn(), TriggerOut());
  REQUIRE(tb_1 != tb_2);

  tb_1 = tb_2;
  REQUIRE(tb_1 == tb_2);

  TriggerOut to_1 = TriggerOut{._pulse_duration = 42., ._polarity = TriggerOut::Polarity::NEGATIVE, ._time_offset = 100.};
  TriggerOut to_2 = TriggerOut{._pulse_duration = 42., ._polarity = TriggerOut::Polarity::NEGATIVE};
  TriggerIn ti_1 = TriggerIn(TriggerLocation{._location = TriggerLocation::Location::EXT, ._id = 0},
                             TriggerIn::TriggerType::HIGH);
  TriggerIn ti_2 = TriggerIn(TriggerLocation{._location = TriggerLocation::Location::EXT, ._id = 0},
                             TriggerIn::TriggerType::RISING);
  tb_2 = TriggerBase(ti_1, to_1);

  tb_1.setTriggerIn(ti_1);
  REQUIRE(tb_1 != tb_2);
  tb_1.setTriggerOut(to_1);
  REQUIRE(tb_1 == tb_2);
  tb_1.setTriggerIn(std::nullopt);
  REQUIRE(tb_1 != tb_2);

  tb_1 = TriggerBase(ti_2, to_2);
  REQUIRE(tb_1 != tb_2);
  tb_1.setTriggerIn(ti_1);
  REQUIRE(tb_1 != tb_2);
  tb_1.setTriggerOut(to_1);
  REQUIRE(tb_1 == tb_2);
  tb_1.setTriggerIn(ti_2);
  REQUIRE(tb_1 != tb_2);
}

}  // namespace uff::test
