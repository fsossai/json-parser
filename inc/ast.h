#pragma once

#include <memory>
#include <string>
#include <vector>

#include "scanner.h"

namespace json_parser {

class BaseVisitor;

class AST {
public:
  virtual ~AST() = default;
  virtual void* Accept(BaseVisitor& visitor);
  virtual bool Parse(Scanner& scanner) = 0;
  bool From(const std::string& input);

  std::vector<std::unique_ptr<AST>> children;
};

class File : public AST {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Object : public AST {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Array : public AST {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Member : public AST {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Value : public AST {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Name : public AST {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;

  std::string text;
};

class Literal : public AST {
public:
  enum class Type {
    INT, FLOAT, STRING, BOOL, NULLTYPE
  };

  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;

  std::string text;
  Type type;
};

}
