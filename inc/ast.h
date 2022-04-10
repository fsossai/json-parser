#pragma once

#include <memory>
#include <string>
#include <vector>

#include "scanner.h"

namespace json_parser {

class BaseVisitor;

class ASTNode {
public:
  virtual ~ASTNode() = default;
  virtual void* Accept(BaseVisitor& visitor);
  virtual bool Parse(Scanner& scanner) = 0;
  virtual std::string ToString() const;

  std::vector<std::unique_ptr<ASTNode>> children;
protected:
  std::string str_;
};

class AST : public ASTNode {
public:
  AST(const std::string& input);
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
  bool Build();
private:
  Scanner scanner_;
};

class Object : public ASTNode {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Array : public ASTNode {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Member : public ASTNode {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Value : public ASTNode {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
};

class Name : public ASTNode {
public:
  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
  std::string name;
};

class Literal : public ASTNode {
public:
  enum class Type {
    INT, FLOAT, STRING
  };

  virtual void* Accept(BaseVisitor& visitor) override;
  bool Parse(Scanner& scanner) override;
  Type GetType() const;

  union {
    int Int;
    float Float;
  } NumericData;

protected:
  Type type_;
};

}
