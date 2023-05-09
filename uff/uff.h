/*!
 * Copyright Moduleus
 * \file uff.h
 * \brief
 */

#ifndef UFF_H
#define UFF_H

#include <string>
#include <type_traits>

namespace uff {

#define UFF_VERSION_MAJOR 0
#define UFF_VERSION_MINOR 2
#define UFF_VERSION_PATCH 0

using MetadataType = double;

// Macro to prevent some class from being copied
#define UFF_MAKE_NONCOPYABLE(ClassName) \
 private:                               \
  ClassName(const ClassName&) = delete; \
  (ClassName)& operator=(const ClassName&) = delete;

#define UFF_TYPE_MACRO(thisClass, superClass)                               \
 protected:                                                                 \
  const char* getClassNameInternal() const override { return #thisClass; }; \
                                                                            \
 public:                                                                    \
  using superclass = superClass;

}  // namespace uff

#endif  // UFF_H