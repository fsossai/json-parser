#include <fstream>
#include <iostream>
#include <sstream>

#include "json_parser.h"

class PrettifyVisitor : public json_parser::BaseVisitor {
public:

  PrettifyVisitor(int nspaces = 4, char space = ' ') {
    indent_n_ = nspaces;
    indent_char_ = space;
  }

  virtual void* Visit(json_parser::AST& ast) override {
    if (ast.children.size() > 0) {
      return ast.children[0]->Accept(*this);
    }
    return nullptr;
  }

  virtual void* Visit(json_parser::Object& object) override {
    if (object.children.size() == 0) {
      partial_ += "{}";
      return nullptr;
    }
    
    partial_ += "{\n";

    indentation_++;
    Indent();
    object.children[0]->Accept(*this);
    for (std::size_t i = 1; i < object.children.size(); i++) {
      partial_ += ",\n";
      Indent();
      object.children[i]->Accept(*this);
    }
    indentation_--;

    partial_ += "\n";
    Indent();
    partial_ += "}";
    
    return nullptr;
  }

  virtual void* Visit(json_parser::Array& array) override {
    if (array.children.size() == 0) {
      partial_ += "[]";
      return nullptr;
    }
    
    partial_ += "[\n";

    indentation_++;
    Indent();
    array.children[0]->Accept(*this);
    for (std::size_t i = 1; i < array.children.size(); i++) {
      partial_ += ",\n";
      Indent();
      array.children[i]->Accept(*this);
    }
    indentation_--;

    partial_ += "\n";
    Indent();
    partial_ += "]";

    return nullptr;
  }

  virtual void* Visit(json_parser::Member& member) override {
    for (auto& child : member.children) {
      child->Accept(*this);
    }
    return nullptr;
  }

  virtual void* Visit(json_parser::Name& name) override {
    partial_ += name.ToString() + ": ";
    return nullptr;
  }

  virtual void* Visit(json_parser::Value& value) override {
    return value.children[0]->Accept(*this);
  }

  virtual void* Visit(json_parser::Literal& literal) override {
    partial_ += literal.ToString();

    for (auto& child : literal.children) {
      child->Accept(*this);
    }

    return nullptr;
  }

  std::string GetResult() const {
    return partial_;
  }

protected:
  void Indent() {
    partial_ += std::string(indentation_ * indent_n_, indent_char_);
  }

  char indent_char_ = ' ';
  int indent_n_ = 4;
  int indentation_ = 0;
  std::string partial_;
};

int main(int argc, char **argv) {
  std::stringstream input;
  
  if (argc > 1) {
    std::ifstream file(argv[1]);
    input << file.rdbuf();
  } else {
    input << std::cin.rdbuf();
  }

  json_parser::AST ast(input.str());
  if (!ast.Build()) {
    std::cerr << "\e[0;31mERROR \e[0m: input text is not in JSON format" << std::endl;
    return 1;
  }

  PrettifyVisitor prettifyVisitor;
  ast.Accept(prettifyVisitor);
  
  std::cout << prettifyVisitor.GetResult();

  return 0;
}
