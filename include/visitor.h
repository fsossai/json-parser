#pragma once

namespace json_parser {

// Forward definitions

class File;
class Object;
class Array;
class Member;
class Name;
class Value;
class Literal;

class Visitor {
public:
  virtual void* Visit(File&);
  virtual void* Visit(Object&);
  virtual void* Visit(Array&);
  virtual void* Visit(Member&);
  virtual void* Visit(Name&);
  virtual void* Visit(Value&);
  virtual void* Visit(Literal&);
};

class ConstVisitor {
public:
  virtual void* Visit(const File&);
  virtual void* Visit(const Object&);
  virtual void* Visit(const Array&);
  virtual void* Visit(const Member&);
  virtual void* Visit(const Name&);
  virtual void* Visit(const Value&);
  virtual void* Visit(const Literal&);
};

}
