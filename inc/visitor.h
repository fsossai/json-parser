#pragma once

#include "parser.h"

namespace JSON {

/* Forward definitions */

class NonTerminal;
class File;
class Object;
class Array;
class Member;
class Name;
class Value;
class Literal;

class BaseVisitor {
public:
  virtual void* Visit(NonTerminal&);
  virtual void* Visit(File&);
  virtual void* Visit(Object&);
  virtual void* Visit(Array&);
  virtual void* Visit(Member&);
  virtual void* Visit(Name&);
  virtual void* Visit(Value&);
  virtual void* Visit(Literal&);
};

}