#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Scanner.h"
#include "Visitor.h"

namespace json_parser {

class Visitor;

class AST {
public:
  virtual ~AST() = default;
  virtual void* Accept(Visitor& visitor) = 0;
  virtual void* Accept(ConstVisitor& visitor) const = 0;
  virtual bool Parse(Scanner& scanner) = 0;
  virtual std::string ToString() const;
  bool From(const std::string& input);
};

class File : public AST {
public:
  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::unique_ptr<AST> file;
};

class Object : public AST {
public:
  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::vector<std::unique_ptr<Member>> members;
  std::map<std::string, Value*> valueMap;
};

class Array : public AST {
public:
  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::vector<std::unique_ptr<Value>> values;
};

class Member : public AST {
public:
  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::unique_ptr<Name> name;
  std::unique_ptr<Value> value;
};

class Value : public AST {
public:
  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::unique_ptr<AST> value;
};

class Name : public AST {
public:
  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::string text;
};

class Literal : public AST {
public:
  enum class Type {
    INT, FLOAT, STRING, BOOL, NULLTYPE
  };

  virtual void* Accept(Visitor& visitor) override;
  virtual void* Accept(ConstVisitor& visitor) const override;
  bool Parse(Scanner& scanner) override;

  std::string text;
  Type type;
};

}
