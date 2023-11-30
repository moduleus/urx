/*!
 * Copyright Moduleus
 * \file uff/object.h
 * \brief
 */

#ifndef UFF_OBJECT_H
#define UFF_OBJECT_H

#include <iosfwd>
#include <limits>  // IWYU pragma: keep
#include <string>

// Standard indentation
#define UFF_STD_INDENT "  "
#define UNDEFINED "UNDEFINED"
#define UFF_NAN std::numeric_limits<MetadataType>::quiet_NaN()

namespace uff::v0_5 {

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

  virtual void printSelf(std::ostream& os, const std::string& indent) const;

  virtual void printHeader(std::ostream& os, const std::string& indent) const;

  virtual void printTrailer(std::ostream& os, const std::string& indent) const;

 private:
  friend std::ostream& operator<<(std::ostream& os, const uff::Object& o);
};

}  // namespace uff::v0_5

#endif  // UFF_OBJECT_H