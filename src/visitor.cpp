
#include "visitor.h"

namespace json_parser {

void* Visitor::Visit(File&)     { return nullptr; }
void* Visitor::Visit(Object&)   { return nullptr; }
void* Visitor::Visit(Array&)    { return nullptr; }
void* Visitor::Visit(Member&)   { return nullptr; }
void* Visitor::Visit(Name&)     { return nullptr; }
void* Visitor::Visit(Value&)    { return nullptr; }
void* Visitor::Visit(Literal&)  { return nullptr; }

void* ConstVisitor::Visit(const File&)     { return nullptr; }
void* ConstVisitor::Visit(const Object&)   { return nullptr; }
void* ConstVisitor::Visit(const Array&)    { return nullptr; }
void* ConstVisitor::Visit(const Member&)   { return nullptr; }
void* ConstVisitor::Visit(const Name&)     { return nullptr; }
void* ConstVisitor::Visit(const Value&)    { return nullptr; }
void* ConstVisitor::Visit(const Literal&)  { return nullptr; }

}
