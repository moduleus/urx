/*!
 * Copyright Moduleus
 * \file uff_object.cpp
 * \brief
 */

#include "uff_object.h"
 
namespace uff 
{

const char* Object::getClassName() const
{
    return this->getClassNameInternal();
}
    
void Object::print(std::ostream& os) const
{
  std::string indent;

  this->printHeader(os, "");
  this->printSelf(os, indent + "  ");
  this->printTrailer(os, "");
}

void Object::printHeader(std::ostream& os, std::string indent) const
{
    os << indent << this->getClassName() << " (" << this << ")\n";
}

void Object::printSelf(std::ostream& os, std::string indent) const
{
    //os << indent << this->getClassName() << " (" << this << ")\n";
    os << this->getClassName() << " (0x" << this << ")\n";
}

void Object::printTrailer(std::ostream& os, std::string indent) const
{
    os << indent << "\n";
}

/**
 * This operator allows all subclasses of uff::Object to be printed via <<.
 * It in turn invokes the 'print' method, which in turn will invoke the
 * 'printSelf' method that all objects should define, if they have anything
 * interesting to print out.
 */
std::ostream& operator<<(std::ostream& os, const uff::Object& o)
{
  o.print(os);
  return os;
}

} // namespace uff