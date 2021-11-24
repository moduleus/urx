/*!
 * Copyright Moduleus
 * \file uff.h
 * \brief
 */

#ifndef UFF_H
#define UFF_H

#include <string>

/* If UFF is built with Qt support add macro for QDebug */
#define UFF_HAS_QT_SUPPORT
#ifdef UFF_HAS_QT_SUPPORT
//#include <QDebug>
namespace uff
{

}    // namespace uff
#endif // UFF_HAS_QT_SUPPORT

namespace uff
{

#define UFF_VERSION_MAJOR 0
#define UFF_VERSION_MINOR 2
#define UFF_VERSION_PATCH 0

#define UFF_LIB_VERSION "0.0.1"

#define UFF_FORMAT_VERSION "v.0.0.40"

// Macro to prevent some class from being copied
#define UFF_MAKE_NONCOPYABLE(ClassName) \
        private: \
            ClassName(const ClassName&) = delete; \
            ClassName& operator=(const ClassName&) = delete; \

#define UFF_TYPE_MACRO(thisClass, superClass) \
protected: \
const char* getClassNameInternal() const override {return #thisClass;}; \
public: \
typedef superClass superclass;

} // namespace uff

#endif // UFF_H