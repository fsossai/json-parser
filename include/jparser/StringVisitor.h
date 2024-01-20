#pragma once

#include <string>

#include "AST.h"
#include "Visitor.h"

namespace jparser {

class StringVisitor : public ConstVisitor {
public:
  virtual void* Visit(const Document& document) override;
  virtual void* Visit(const Object& object) override;
  virtual void* Visit(const Array& array) override;
  virtual void* Visit(const Member& member) override;
  virtual void* Visit(const Name& name) override;
  virtual void* Visit(const Value& value) override;
  virtual void* Visit(const Literal& literal) override;
  std::string GetResult() const;

protected:
  std::string partial_;
};

}
