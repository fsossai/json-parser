#pragma once

namespace jparser {

// Forward definitions
class Document;
class Object;
class Array;
class Member;
class Name;
class Value;
class Literal;

class Visitor {
public:
  virtual void *Visit(Document&) = 0;
  virtual void *Visit(Object&) = 0;
  virtual void *Visit(Array&) = 0;
  virtual void *Visit(Member&) = 0;
  virtual void *Visit(Name&) = 0;
  virtual void *Visit(Value&) = 0;
  virtual void *Visit(Literal&) = 0;
};

class ConstVisitor {
public:
  virtual void *Visit(const Document&) = 0;
  virtual void *Visit(const Object&) = 0;
  virtual void *Visit(const Array&) = 0;
  virtual void *Visit(const Member&) = 0;
  virtual void *Visit(const Name&) = 0;
  virtual void *Visit(const Value&) = 0;
  virtual void *Visit(const Literal&) = 0;
};

}
