/*!
 * Copyright Moduleus
 * \file urx.h
 * \brief
 */

#ifndef URX_H
#define URX_H

#include <string>
#include <type_traits>

namespace urx::v0_5 {

constexpr int URX_VERSION_MAJOR = 0;
constexpr int URX_VERSION_MINOR = 2;
constexpr int URX_VERSION_PATCH = 0;

using MetadataType = double;

// Macro to prevent some class from being copied
#define URX_MAKE_NONCOPYABLE(ClassName) \
 private:                               \
  ClassName(const ClassName&) = delete; \
  (ClassName)& operator=(const ClassName&) = delete;

#define URX_TYPE_MACRO(thisClass, superClass)                               \
 protected:                                                                 \
  const char* getClassNameInternal() const override { return #thisClass; }; \
                                                                            \
 public:                                                                    \
  using superclass = superClass;

}  // namespace urx::v0_5

#endif  // URX_H