#pragma once

#include <memory>
#include <string>

#include "scanner.h"

namespace JSON {

class NonTerminal;

class BaseVisitor {
public:
  virtual void* Visit(NonTerminal& nt);
};

class NonTerminal {
public:
  virtual void* Accept(BaseVisitor visitor);
  virtual std::string ToString() const;
  virtual bool Parse(Scanner& scanner) = 0;
protected:
  std::string str_;
};

class File : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse(Scanner& scanner) override;
};

class Object : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse(Scanner& scanner) override;
};

class Array : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse(Scanner& scanner) override;
};

class Member : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse(Scanner& scanner) override;
};

class Value : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse(Scanner& scanner) override;
};

class Literal : public NonTerminal {
public:
  enum class Type {
    INT, FLOAT, STRING
  };

  using NonTerminal::NonTerminal;
  bool Parse(Scanner& scanner) override;
  Type GetType() const;
  
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
