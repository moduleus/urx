#include <ios>
#include <ostream>
#include <string>
#include <utility>

#include <urx/v0_2/types.h>

namespace urx::v0_2 {

std::ostream& operator<<(std::ostream& out, const WaveType& value) {
  std::string s;
#define PROCESS_VAL(p) \
  case (p):            \
    s = #p;            \
    break;
  switch (value) {
    PROCESS_VAL(WaveType::CONVERGING_WAVE);
    PROCESS_VAL(WaveType::DIVERGING_WAVE);
    PROCESS_VAL(WaveType::PLANE_WAVE);
    PROCESS_VAL(WaveType::CYLINDRICAL_WAVE);
  }
#undef PROCESS_VAL

  return out << s;
}  // namespace urx::v0_2

}  // namespace urx::v0_2