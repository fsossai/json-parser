#pragma once

#include <string>

#include "AST.h"
#include "Visitor.h"

namespace jparser {

class StringVisitor : public ConstVisitor {
public:
  void *Visit(const Document& document) override;
  void *Visit(const StreamDocument& document) override;
  void *Visit(const Object& object) override;
  void *Visit(const Array& array) override;
  void *Visit(const Member& member) override;
  void *Visit(const Name& name) override;
  void *Visit(const Value& value) override;
  void *Visit(const Literal& literal) override;
  std::string GetResult() const;

protected:
  std::string partial_;
};

}
