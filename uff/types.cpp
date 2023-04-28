#include "types.h"

#include <string>

namespace uff {

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
}  // namespace uff

}  // namespace uff