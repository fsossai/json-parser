#include <fstream>
#include <iostream>
#include <sstream>

#include "StringVisitor.h"

using namespace std;

namespace json_parser {

void* StringVisitor::Visit(const File& file) {
  if (file.children.size() > 0) {
    return file.children[0]->Accept(*this);
  }
  return nullptr;
}

void* StringVisitor::Visit(const Object& object) {
  if (object.children.size() == 0) {
    partial_ += "{}";
    return nullptr;
  }
  
  partial_ += "{";
  object.children[0]->Accept(*this);
  for (size_t i = 1; i < object.children.size(); i++) {
    partial_ += ",";
    object.children[i]->Accept(*this);
  }
  partial_ += "}";
  
  return nullptr;
}

void* StringVisitor::Visit(const Array& array) {
  if (array.children.size() == 0) {
    partial_ += "[]";
    return nullptr;
  }
  
  partial_ += "[";
  array.children[0]->Accept(*this);
  for (size_t i = 1; i < array.children.size(); i++) {
    partial_ += ",";
    array.children[i]->Accept(*this);
  }
  partial_ += "]";

  return nullptr;
}

void* StringVisitor::Visit(const Member& member) {
  if (member.children.size() < 2) {
    return nullptr;
  }
  member.children[0]->Accept(*this); // Name
  partial_ += ":";
  member.children[1]->Accept(*this); // Value
  return nullptr;
}

void* StringVisitor::Visit(const Name& name) {
  partial_ += "\"" + name.text + "\"";
  return nullptr;
}

void* StringVisitor::Visit(const Value& value) {
  if (value.children.size() == 0) {
    return nullptr;
  }
  return value.children[0]->Accept(*this);
}

void* StringVisitor::Visit(const Literal& literal) {
  partial_ += literal.text;
  return nullptr;
}

string StringVisitor::GetResult() const {
  return partial_;
}

}

