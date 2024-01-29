#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "jparser/jparser.h"

using namespace std;
using namespace jparser;

using Palette = unordered_map<string, string>;

class ColorVisitor : public ConstVisitor {
public:

  ColorVisitor(int nspaces = 4, char space = ' ')
      : indent_n_(nspaces)
      , indent_char_(space) {
    ResetPalette();
  }

  void SetPalette(Palette palette) {
    palette_ = palette;
  }

  Palette GetDefaultPalette() const {
    Palette palette{
      {",", "1"},
      {":", "1"},
      {"{", "1"},
      {"}", "1"},
      {"[", "1"},
      {"]", "1"},
      {"int", "0"},
      {"float", "0"},
      {"string", "32"},
      {"bool", "0"},
      {"null", "90"},
      {"name", "1;94"}
    };
    return palette;
  }

  void ResetPalette() {
    palette_ = GetDefaultPalette(); 
  }

  string ToColor(string name) {
    return "\e[0;" + palette_[name] + "m";
  }

  string Stop() const {
    return "\e[0m";
  }

  void *Visit(const Document& document) override {
    if (document.document) {
      return document.document->Accept(*this);
    }
    return nullptr;
  }

  void *Visit(const StreamDocument& document) override {
    if (document.values.size() > 0) {
      document.values[0]->Accept(*this);
    }
    
    for (size_t i = 1; i < document.values.size(); i++) {
      partial_ += "\n";
      document.values[i]->Accept(*this);
    }
    return nullptr;
  }

  void *Visit(const Object& object) override {
    if (object.members.size() == 0) {
      partial_ += ToColor("{") + "{" + Stop();
      partial_ += ToColor("}") + "}" + Stop();
      return nullptr;
    }
    partial_ += ToColor("{") + "{" + Stop() + "\n";
    indentation_++;
    Indent();

    object.members[0]->Accept(*this);
    for (size_t i = 1; i < object.members.size(); i++) {
      partial_ += ToColor(",") + "," + Stop() + "\n";
      Indent();
      object.members[i]->Accept(*this);
    }
    indentation_--;

    partial_ += "\n";
    Indent();
    partial_ += ToColor("}") + "}" + Stop();

    return nullptr;
  }

  void *Visit(const Array& array) override {
    if (array.values.size() == 0) {
      partial_ += ToColor("[") + "[" + Stop();
      partial_ += ToColor("]") + "]" + Stop();
      return nullptr;
    }
    
    partial_ += ToColor("[") + "[" + Stop() + "\n";

    indentation_++;
    Indent();
    array.values[0]->Accept(*this);
    for (size_t i = 1; i < array.values.size(); i++) {
      partial_ += ToColor(",") + ",\n" + Stop();
      Indent();
      array.values[i]->Accept(*this);
    }
    indentation_--;

    partial_ += "\n";
    Indent();
    partial_ += ToColor("]") + "]" + Stop();

    return nullptr;
  }

  void *Visit(const Member& member) override {
    if (member.name) {
      member.name->Accept(*this);
    }
    if (member.value) {
      partial_ += ToColor(":") + ": " + Stop();
      member.value->Accept(*this);
    }
    return nullptr;
  }

  void *Visit(const Name& name) override {
    partial_ += ToColor("name") + "\"" + name.text + "\"" + Stop();
    return nullptr;
  }

  void *Visit(const Value& value) override {
    return value.value->Accept(*this);
  }

  void *Visit(const Literal& literal) override {
    string name;
    switch (literal.type) {
    case Literal::Type::INT:
      name = "int";
      break;
    case Literal::Type::FLOAT:
      name = "float";
      break;
    case Literal::Type::STRING:
      name = "string";
      break;
    case Literal::Type::BOOL:
      name = "bool";
      break;
    case Literal::Type::NULLTYPE:
      name = "null";
      break;
    }
    partial_ += ToColor(name) + literal.text + Stop();
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
  Palette palette_;
};

Palette ReadPalette(const string& filename) {
  stringstream ss;
  ifstream ifs(filename);

  Palette palette;
  Object object;

  if (ifs.is_open()) {
    ss << ifs.rdbuf();
    if (object.From(ss.str())) {
      for (const auto& member : object.members) {
        auto name = member->name->text;
        auto value = member->value->ToString();
        palette[name] = value.substr(1, value.size() - 2);
      }
    }
  }
  return palette;
}

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
    cerr << "\e[0;31mERROR\e[0m: input text is not in JSON format" << endl;
    return 1;
  }

  int nspaces = 2;
  int space = ' ';
  ColorVisitor colorVisitor(nspaces, space);
  auto palette = ReadPalette("palette.json");

  if (palette.size() > 0) {
    colorVisitor.SetPalette(palette);
  }

  document.Accept(colorVisitor);
  cout << colorVisitor.GetResult() << "\n";

  return 0;
}
