#include <fstream>
#include <iostream>
#include <sstream>

#include "StringVisitor.h"

using namespace std;

namespace json_parser {

void* StringVisitor::Visit(const File& file) {
  if (file.file) {
    return file.file->Accept(*this);
  }
  return nullptr;
}

void* StringVisitor::Visit(const Object& object) {
  if (object.members.size() == 0) {
    partial_ += "{}";
    return nullptr;
  }
  
  partial_ += "{";
  object.members[0]->Accept(*this);
  for (size_t i = 1; i < object.members.size(); i++) {
    partial_ += ",";
    object.members[i]->Accept(*this);
  }
  partial_ += "}";
  
  return nullptr;
}

void* StringVisitor::Visit(const Array& array) {
  if (array.values.size() == 0) {
    partial_ += "[]";
    return nullptr;
  }
  
  partial_ += "[";
  array.values[0]->Accept(*this);
  for (size_t i = 1; i < array.values.size(); i++) {
    partial_ += ",";
    array.values[i]->Accept(*this);
  }
  partial_ += "]";

  return nullptr;
}

void* StringVisitor::Visit(const Member& member) {
  if (member.name) {
    member.name->Accept(*this);
  }
  if (member.value) {
    partial_ += ":";
    member.value->Accept(*this);
  }
  return nullptr;
}

void* StringVisitor::Visit(const Name& name) {
  partial_ += "\"" + name.text + "\"";
  return nullptr;
}

void* StringVisitor::Visit(const Value& value) {
  if (value.value) {
    value.value->Accept(*this);
  }
  return nullptr;
}

void* StringVisitor::Visit(const Literal& literal) {
  partial_ += literal.text;
  return nullptr;
}

string StringVisitor::GetResult() const {
  return partial_;
}

}

