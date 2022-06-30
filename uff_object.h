/*!
 * Copyright Moduleus
 * \file uff_object.h
 * \brief
 */

#ifndef UFF_OBJECT_H
#define UFF_OBJECT_H

// UFF
#include "uff.h"

// System
#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Standard indentation
#define UFF_STD_INDENT "  "
#define UNDEFINED "UNDEFINED"
#define UFF_NAN std::numeric_limits<FloatingType>::quiet_NaN()

namespace uff {

/**
 * The uff::Object class is the base class for all UFF components.
 */
class Object {
 public:
  Object() = default;
  virtual ~Object() = default;

 protected:
  virtual const char* getClassNameInternal() const { return "Object"; }

 public:
  const char* getClassName() const;

  void print(std::ostream& os) const;

  virtual void printSelf(std::ostream& os, std::string indent) const;

  virtual void printHeader(std::ostream& os, std::string indent) const;

  virtual void printTrailer(std::ostream& os, std::string indent) const;

 private:
  friend std::ostream& operator<<(std::ostream& os, const uff::Object& o);
};

}  // namespace uff

#endif  // UFF_OBJECT_H