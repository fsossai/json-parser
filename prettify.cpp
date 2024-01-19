#include <fstream>
#include <iostream>
#include <sstream>

#include "json_parser.h"

using namespace std;

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
    for (size_t i = 1; i < object.children.size(); i++) {
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
    for (size_t i = 1; i < array.children.size(); i++) {
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
    member.children[0]->Accept(*this); // Name
    partial_ += ": ";
    member.children[1]->Accept(*this); // Value
    return nullptr;
  }

  virtual void* Visit(json_parser::Name& name) override {
    partial_ += "\"" + name.text + "\"";
    return nullptr;
  }

  virtual void* Visit(json_parser::Value& value) override {
    return value.children[0]->Accept(*this);
  }

  virtual void* Visit(json_parser::Literal& literal) override {
    partial_ += literal.text;
    return nullptr;
  }

  string GetResult() const {
    return partial_;
  }

protected:
  void Indent() {
    partial_ += string(indentation_ * indent_n_, indent_char_);
  }

  char indent_char_ = ' ';
  int indent_n_ = 4;
  int indentation_ = 0;
  string partial_;
};

int main(int argc, char **argv) {
  stringstream input;
  
  if (argc > 1) {
    ifstream file(argv[1]);
    input << file.rdbuf();
  } else {
    input << cin.rdbuf();
  }

  json_parser::File file;

  if (!file.From(input.str())) {
    cerr << "\e[0;31mERROR \e[0m: input text is not in JSON format" << endl;
    return 1;
  }

  PrettifyVisitor prettifyVisitor;
  file.Accept(prettifyVisitor);
  
  cout << prettifyVisitor.GetResult();

  return 0;
}
