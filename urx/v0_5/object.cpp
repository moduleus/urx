/*!
 * Copyright Moduleus
 * \file urx/object.cpp
 * \brief
 */

#include <urx/v0_5/object.h>

#include <memory>
#include <ostream>
#include <utility>

namespace urx::v0_5 {

const char* Object::getClassName() const { return this->getClassNameInternal(); }

void Object::print(std::ostream& os) const {
  std::string indent;

  this->printHeader(os, "");
  this->printSelf(os, indent + "  ");
  this->printTrailer(os, "");
}

void Object::printHeader(std::ostream& os, const std::string& indent) const {
  os << indent << this->getClassName() << " (" << this << ")\n";
}

void Object::printSelf(std::ostream& os, const std::string& indent) const {
  //os << indent << this->getClassName() << " (" << this << ")\n";
  os << this->getClassName() << " (0x" << this << ")\n";
}

void Object::printTrailer(std::ostream& os, const std::string& indent) const {
  os << indent << "\n";
}

/**
 * This operator allows all subclasses of urx::Object to be printed via <<.
 * It in turn invokes the 'print' method, which in turn will invoke the
 * 'printSelf' method that all objects should define, if they have anything
 * interesting to print out.
 */
std::ostream& operator<<(std::ostream& os, const urx::Object& o) {
  o.print(os);
  return os;
}

}  // namespace urx::v0_5