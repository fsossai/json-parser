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
  NonTerminal(Scanner& scanner);
  virtual void* Accept(BaseVisitor visitor);
  virtual std::string ToString() const;
  virtual bool Parse() = 0;
protected:
  std::string str_;
  Scanner& scanner_;
};

class File : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse() override;
};

class Object : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse() override;
};

class Array : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse() override;
};

class Member : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse() override;
};

class Value : public NonTerminal {
public:
  using NonTerminal::NonTerminal;
  bool Parse() override;
};

class Literal : public NonTerminal {
public:
  enum class Type {
    INT, FLOAT, STRING
  };

  using NonTerminal::NonTerminal;
  bool Parse() override;
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
  
protected:
  Scanner& scanner_;
};

}
