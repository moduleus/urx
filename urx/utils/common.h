#pragma once

#include <string>
#include <version>

#include <urx/utils/export.h>

namespace urx::utils::common {

///
///\brief Index to string with leading zeros.
///
///\note Avoid using std::format function that is not yet implemented by all compilers claiming c++20 support.
///
///\param index The index value
///\param length The string output length (minimum)
///\return The index value as a string with as many necessary leading zeros to fill length characters
///
URX_UTILS_EXPORT std::string formatIndexWithLeadingZeros(const std::size_t index, const int length);

}  // namespace urx::utils::common
