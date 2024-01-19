
#include "visitor.h"

namespace json_parser {

void* Visitor::Visit(ASTNode&)  { return nullptr; }
void* Visitor::Visit(AST&)      { return nullptr; }
void* Visitor::Visit(Object&)   { return nullptr; }
void* Visitor::Visit(Array&)    { return nullptr; }
void* Visitor::Visit(Member&)   { return nullptr; }
void* Visitor::Visit(Name&)     { return nullptr; }
void* Visitor::Visit(Value&)    { return nullptr; }
void* Visitor::Visit(Literal&)  { return nullptr; }

}
