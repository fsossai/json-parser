#pragma once

#include <memory>
#include <string>
#include <vector>

#include "scanner.h"

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
  virtual void* Visit(NonTerminal&) = 0;
  virtual void* Visit(File&)        = 0;
  virtual void* Visit(Object&)      = 0;
  virtual void* Visit(Array&)       = 0;
  virtual void* Visit(Member&)      = 0;
  virtual void* Visit(Name&)        = 0;
  virtual void* Visit(Value&)       = 0;
  virtual void* Visit(Literal&)     = 0;
};

class NonTerminal {
public:
  virtual ~NonTerminal() = default;
  virtual void* Accept(BaseVisitor& visitor);
  virtual bool Parse(Scanner& scanner) = 0;
  virtual std::string ToString() const;

  std::vector<std::unique_ptr<NonTerminal>> children;
protected:
  std::string str_;
};

class File : public NonTerminal {
public:
virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Object : public NonTerminal {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Array : public NonTerminal {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Member : public NonTerminal {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Value : public NonTerminal {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Name : public NonTerminal {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
  std::string name;
};

class Literal : public NonTerminal {
public:
  enum class Type {
    INT, FLOAT, STRING
  };

  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
  Type GetType() const;

  void *data;
  
  int IntData;
  float FloatData;
  std::string StringData;

protected:
  Type type_;
};

class Parser {
public:
  Parser(Scanner& scanner);
  File Parse();
  
protected:
  Scanner& scanner_;
};

}
