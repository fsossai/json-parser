#include <memory>
#include <string>

#include "ast.h"
#include "scanner.h"
#include "visitor.h"

namespace json_parser {

void* ASTNode::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

std::string ASTNode::ToString() const {
  return str_;
}

AST::AST(const std::string& input)
    : scanner_(input) {
}

bool AST::Build() {
  return Parse(scanner_);
}

bool AST::Parse(Scanner& scanner) {
  Token tok;
  std::unique_ptr<Object> object = std::make_unique<Object>();
  std::unique_ptr<Array> array = std::make_unique<Array>();
  scanner.PushStatus();
  if (object->Parse(scanner)) {
    tok = scanner.GetNextToken();
    REQUIRE(tok == Token::END);
    
    str_ = object->ToString();
    children.push_back(std::move(object));
    scanner.PopStatus();
    return true;
  }
  scanner.RestoreStatus();
  if (array->Parse(scanner)) {
    tok = scanner.GetNextToken();
    REQUIRE(tok == Token::END);
    
    str_ = array->ToString();
    children.push_back(std::move(array));
    scanner.PopStatus();
    return true;
  }
  fail:
  scanner.RestoreStatus();
  return false;
}

bool Object::Parse(Scanner& scanner) {
  scanner.PushStatus();
  std::string tmp;
  std::unique_ptr<Member> member;
  
  Token tok = scanner.GetNextToken();
  REQUIRE(tok == Token::OBJ_OPEN);

  str_ += scanner.GetLastLexeme();
  member = std::make_unique<Member>();
  if (!member->Parse(scanner)) {
    tok = scanner.GetNextToken();
    goto closing;
  }

  str_ += member->ToString();
  children.push_back(std::move(member));
  tok = scanner.GetNextToken();
  while (tok == Token::COMMA) {
    str_ += scanner.GetLastLexeme();
    member = std::make_unique<Member>();
    REQUIRE(member->Parse(scanner));

    str_ += member->ToString();
    children.push_back(std::move(member));
    tok = scanner.GetNextToken();
  }

  closing:
  REQUIRE(tok == Token::OBJ_CLOSE);
  str_ += scanner.GetLastLexeme();
  scanner.PopStatus();
  return true;

  fail:
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Array::Parse(Scanner& scanner) {
  scanner.PushStatus();
  std::unique_ptr<Value> value;
  
  Token tok = scanner.GetNextToken();
  REQUIRE(tok == Token::ARRAY_OPEN);
  
  str_ += scanner.GetLastLexeme();
  value = std::make_unique<Value>();
  if (!value->Parse(scanner)) {
    tok = scanner.GetNextToken();
    goto closing;
  }
  
  str_ += value->ToString();
  children.push_back(std::move(value));
  tok = scanner.GetNextToken();
  while (tok == Token::COMMA) {
    str_ += scanner.GetLastLexeme();
    value = std::make_unique<Value>();
    REQUIRE(value->Parse(scanner));
    
    str_ += value->ToString();
    children.push_back(std::move(value));
    tok = scanner.GetNextToken();
  }

  closing:
  REQUIRE(tok == Token::ARRAY_CLOSE);
  str_ += scanner.GetLastLexeme();
  scanner.PopStatus();
  return true;

  fail:
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Member::Parse(Scanner& scanner) {
  scanner.PushStatus();
  Token tok;
  std::string name_lexeme;
  std::unique_ptr<Name> name = std::make_unique<Name>();
  std::unique_ptr<Value> value;

  REQUIRE(name->Parse(scanner));
  
  name_lexeme = scanner.GetLastLexeme();
  tok = scanner.GetNextToken();
  REQUIRE(tok == Token::COLON);
  
  value = std::make_unique<Value>();
  REQUIRE(value->Parse(scanner));
  
  str_ = name_lexeme + ": " + value->ToString();
  children.push_back(std::move(name));
  children.push_back(std::move(value));
  scanner.PopStatus();
  return true;
  
  fail:
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Value::Parse(Scanner& scanner) {
  scanner.PushStatus();

  std::unique_ptr<Object> object = std::make_unique<Object>();
  if (object->Parse(scanner)) {
    str_ = object->ToString();
    children.push_back(std::move(object));
    scanner.PopStatus();
    return true;
  }
  scanner.RestoreStatus();

  std::unique_ptr<Array> array = std::make_unique<Array>();
  if (array->Parse(scanner)) {
    str_ = array->ToString();
    children.push_back(std::move(array));
    scanner.PopStatus();
    return true;
  }
  scanner.RestoreStatus();

  std::unique_ptr<Literal> literal = std::make_unique<Literal>();
  if (literal->Parse(scanner)) {
    str_ = literal->ToString();
    children.push_back(std::move(literal));
    scanner.PopStatus();
    return true;
  }

  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Literal::Parse(Scanner& scanner) {
  scanner.PushStatus();
  Token tok = scanner.GetNextToken();
  str_ = scanner.GetLastLexeme();

  switch (tok)
  {
  case Token::INT:
    NumericData.Int = std::stoi(str_);
    type_ = Type::INT;
    break;
  case Token::FLOAT:
    NumericData.Float = std::stof(str_);
    type_ = Type::FLOAT;
    break;
  case Token::STRING:
    type_ = Type::STRING;
    break;
  case Token::BOOL:
    type_ = Type::BOOL;
    break;
  default:
    scanner.RestoreStatus();
    scanner.PopStatus();
    return false;
  }
  
  scanner.PopStatus();
  return true;
}

bool Name::Parse(Scanner& scanner) {
  scanner.PushStatus();
  Token tok = scanner.GetNextToken();
  std::string name_lexeme = scanner.GetLastLexeme();
  REQUIRE(tok == Token::STRING);

  str_ = name = name_lexeme;
  scanner.PopStatus();
  return true;

  fail:
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

Literal::Type Literal::GetType() const {
  return type_;
}

/* 'Accept' overrides */

void* AST::Accept(BaseVisitor& visitor)     { return visitor.Visit(*this); }
void* Object::Accept(BaseVisitor& visitor)  { return visitor.Visit(*this); }
void* Array::Accept(BaseVisitor& visitor)   { return visitor.Visit(*this); }
void* Member::Accept(BaseVisitor& visitor)  { return visitor.Visit(*this); }
void* Name::Accept(BaseVisitor& visitor)    { return visitor.Visit(*this); }
void* Value::Accept(BaseVisitor& visitor)   { return visitor.Visit(*this); }
void* Literal::Accept(BaseVisitor& visitor) { return visitor.Visit(*this); }

}
