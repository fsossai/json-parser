#include <memory>
#include <string>
#include <unordered_set>
#include <iostream>

#include "ast.h"
#include "scanner.h"
#include "visitor.h"

#define REQUIRE(x) if (!(x)) goto fail;

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
  if (scanner.Peek() == Token::OBJ_OPEN) {
    std::unique_ptr<Object> object = std::make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    REQUIRE(scanner.ConsumeToken() == Token::END);
    
    str_ += object->ToString();
    children.push_back(std::move(object));
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    std::unique_ptr<Array> array = std::make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    REQUIRE(scanner.ConsumeToken() == Token::END);
    
    str_ += array->ToString();
    children.push_back(std::move(array));
    return true;
  } else {
    std::cout << TokenToString(scanner.Peek());
  }
fail:
  return false;
}

bool Object::Parse(Scanner& scanner) {
  std::string tmp;
  std::unique_ptr<Member> member;
  std::unordered_set<std::string> keys;

  auto GetMemberName = [&member]() -> auto {
    return member->children[0]->ToString();
  };
  
  REQUIRE(scanner.ConsumeToken() == Token::OBJ_OPEN);

  if (scanner.Peek() == Token::OBJ_CLOSE) {
    scanner.ConsumeToken();
    str_ += scanner.GetLastLexeme();
    return true;
  }

  member = std::make_unique<Member>();
  REQUIRE(member->Parse(scanner));

  while (scanner.Peek() == Token::COMMA) {
    scanner.ConsumeToken();
    member = std::make_unique<Member>();
    REQUIRE(member->Parse(scanner));
    REQUIRE(keys.find(GetMemberName()) == keys.end());
    keys.insert(GetMemberName());
    str_ += member->ToString();
    children.push_back(std::move(member));
  }

  REQUIRE(scanner.ConsumeToken() == Token::OBJ_CLOSE);
  str_ += scanner.GetLastLexeme();
  return true;

fail:
  return false;
}

bool Array::Parse(Scanner& scanner) {
  std::unique_ptr<Value> value;

  REQUIRE(scanner.ConsumeToken() == Token::ARRAY_OPEN);
  str_ += scanner.GetLastLexeme();

  if (scanner.Peek() == Token::ARRAY_CLOSE) {
    scanner.ConsumeToken();
    str_ += scanner.GetLastLexeme();
    return true;
  }
  
  value = std::make_unique<Value>();
  REQUIRE(value->Parse(scanner));
  str_ += value->ToString();
  children.push_back(std::move(value));

  while (scanner.Peek() == Token::COMMA) {
    scanner.ConsumeToken();
    value = std::make_unique<Value>();
    REQUIRE(value->Parse(scanner));
    str_ += value->ToString();
    children.push_back(std::move(value));
  }

  REQUIRE(scanner.ConsumeToken() == Token::ARRAY_CLOSE);
  str_ += scanner.GetLastLexeme();
  return true;

fail:
  return false;
}

bool Member::Parse(Scanner& scanner) {
  std::string name_lexeme;
  std::unique_ptr<Value> value;
  std::unique_ptr<Name> name;

  name = std::make_unique<Name>();
  REQUIRE(name->Parse(scanner));
  str_ += name->ToString();
  REQUIRE(scanner.ConsumeToken() == Token::COLON);
  str_ += scanner.GetLastLexeme();

  value = std::make_unique<Value>();
  REQUIRE(value->Parse(scanner));
  str_ += value->ToString();

  children.push_back(std::move(name));
  children.push_back(std::move(value));

  return true;
  
fail:
  return false;
}

bool Value::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJ_OPEN) {
    std::unique_ptr<Object> object = std::make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    
    str_ += object->ToString();
    children.push_back(std::move(object));
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    std::unique_ptr<Array> array = std::make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    
    str_ += array->ToString();
    children.push_back(std::move(array));
    return true;
  }

  {
    std::unique_ptr<Literal> literal = std::make_unique<Literal>();
    REQUIRE(literal->Parse(scanner));
    str_ += literal->ToString();
    return true;
  }

fail:
  return false;
}

bool Literal::Parse(Scanner& scanner) {
  switch (scanner.ConsumeToken()) {
  case Token::INT:
    type_ = Type::INT;
    break;
  case Token::FLOAT:
    type_ = Type::FLOAT;
    break;
  case Token::STRING:
    type_ = Type::STRING;
    break;
  case Token::BOOL:
    type_ = Type::BOOL;
    break;
  case Token::NULLTOKEN:
    type_ = Type::NULLTYPE;
    break;
  default:
    return false;
  }

  str_ += scanner.GetLastLexeme();
  return true;
}

bool Name::Parse(Scanner& scanner) {
  REQUIRE(scanner.ConsumeToken() == Token::STRING);
  str_ += scanner.GetLastLexeme();
  return true;

fail:
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
