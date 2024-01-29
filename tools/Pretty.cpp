#include <fstream>
#include <iostream>
#include <sstream>

#include "jparser/jparser.h"

using namespace std;
using namespace jparser;

class PrettyVisitor : public Visitor {
public:

  PrettyVisitor(int nspaces = 4, char space = ' ') {
    indent_n_ = nspaces;
    indent_char_ = space;
  }

  void* Visit(Document& document) override {
    if (document.document) {
      return document.document->Accept(*this);
    }
    return nullptr;
  }

  void* Visit(StreamDocument& document) override {
    if (document.values.size() > 0) {
      document.values[0]->Accept(*this);
    }
    
    for (size_t i = 1; i < document.values.size(); i++) {
      partial_ += "\n";
      document.values[i]->Accept(*this);
    }
    return nullptr;
  }

  void* Visit(Object& object) override {
    if (object.members.size() == 0) {
      partial_ += "{}";
      return nullptr;
    }
    
    partial_ += "{\n";

    indentation_++;
    Indent();
    object.members[0]->Accept(*this);
    for (size_t i = 1; i < object.members.size(); i++) {
      partial_ += ",\n";
      Indent();
      object.members[i]->Accept(*this);
    }
    indentation_--;

    partial_ += "\n";
    Indent();
    partial_ += "}";
    
    return nullptr;
  }

  void* Visit(Array& array) override {
    if (array.values.size() == 0) {
      partial_ += "[]";
      return nullptr;
    }
    
    partial_ += "[\n";

    indentation_++;
    Indent();
    array.values[0]->Accept(*this);
    for (size_t i = 1; i < array.values.size(); i++) {
      partial_ += ",\n";
      Indent();
      array.values[i]->Accept(*this);
    }
    indentation_--;

    partial_ += "\n";
    Indent();
    partial_ += "]";

    return nullptr;
  }

  void* Visit(Member& member) override {
    if (member.name) {
      member.name->Accept(*this);
    }
    if (member.value) {
      partial_ += ": ";
      member.value->Accept(*this);
    }
    return nullptr;
  }

  void* Visit(Name& name) override {
    partial_ += "\"" + name.text + "\"";
    return nullptr;
  }

  void* Visit(Value& value) override {
    return value.value->Accept(*this);
  }

  void* Visit(Literal& literal) override {
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
    ifstream ifs(argv[1]);
    input << ifs.rdbuf();
  } else {
    input << cin.rdbuf();
  }

  StreamDocument document;

  if (!document.From(input.str())) {
    cerr << "\e[0;31mERROR \e[0m: input text is not in JSON format" << endl;
    return 1;
  }

  int nspaces = 4;
  int space = ' ';
  PrettyVisitor prettyVisitor(nspaces, space);
  document.Accept(prettyVisitor);
  
  cout << prettyVisitor.GetResult() << "\n";

  return 0;
}
