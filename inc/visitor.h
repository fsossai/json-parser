#pragma once

namespace json_parser {

/* Forward definitions */

class ASTNode;
class AST;
class Object;
class Array;
class Member;
class Name;
class Value;
class Literal;

class BaseVisitor {
public:
  virtual void* Visit(ASTNode&);
  virtual void* Visit(AST&);
  virtual void* Visit(Object&);
  virtual void* Visit(Array&);
  virtual void* Visit(Member&);
  virtual void* Visit(Name&);
  virtual void* Visit(Value&);
  virtual void* Visit(Literal&);
};

}