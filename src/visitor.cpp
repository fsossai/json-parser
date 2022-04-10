
#include "visitor.h"

namespace json_parser {

void* BaseVisitor::Visit(ASTNode&)  { return nullptr; }
void* BaseVisitor::Visit(AST&)      { return nullptr; }
void* BaseVisitor::Visit(Object&)   { return nullptr; }
void* BaseVisitor::Visit(Array&)    { return nullptr; }
void* BaseVisitor::Visit(Member&)   { return nullptr; }
void* BaseVisitor::Visit(Name&)     { return nullptr; }
void* BaseVisitor::Visit(Value&)    { return nullptr; }
void* BaseVisitor::Visit(Literal&)  { return nullptr; }

}
