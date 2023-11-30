/*!
 * Copyright Moduleus
 * \file urx/object.h
 * \brief
 */

#ifndef URX_OBJECT_H
#define URX_OBJECT_H

#include <iosfwd>
#include <limits>  // IWYU pragma: keep
#include <string>

// Standard indentation
#define URX_STD_INDENT "  "
#define UNDEFINED "UNDEFINED"
#define URX_NAN std::numeric_limits<MetadataType>::quiet_NaN()

namespace urx::v0_5 {

/**
 * The urx::Object class is the base class for all URX components.
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
  friend std::ostream& operator<<(std::ostream& os, const urx::Object& o);
};

}  // namespace urx::v0_5

#endif  // URX_OBJECT_H