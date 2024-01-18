#include <memory>
#include <string>
#include <unordered_set>
#include <iostream>

#include "ast.h"
#include "scanner.h"
#include "visitor.h"

#define REQUIRE(x) if (!(x)) goto fail;

namespace json_parser {

bool File::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    std::unique_ptr<Object> object = std::make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    
    children.push_back(std::move(object));
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    std::unique_ptr<Array> array = std::make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    
    children.push_back(std::move(array));
    return true;
  }

fail:
  return false;
}

bool Object::Parse(Scanner& scanner) {
  std::string tmp;
  std::unique_ptr<Member> member;
  std::unordered_set<std::string> keys;

  auto GetMemberName = [&member]() -> auto {
    return static_cast<Name*>(member->children[0].get())->text;
  };
  
  REQUIRE(scanner.Consume() == Token::OBJECT_OPEN);

  if (scanner.Peek() == Token::OBJECT_CLOSE) {
    scanner.Consume();
    return true;
  }

  member = std::make_unique<Member>();
  REQUIRE(member->Parse(scanner));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    member = std::make_unique<Member>();
    REQUIRE(member->Parse(scanner));
    REQUIRE(keys.find(GetMemberName()) == keys.end());
    keys.insert(GetMemberName());
    children.push_back(std::move(member));
  }

  REQUIRE(scanner.Consume() == Token::OBJECT_CLOSE);
  return true;

fail:
  return false;
}

bool Array::Parse(Scanner& scanner) {
  std::unique_ptr<Value> value;

  REQUIRE(scanner.Consume() == Token::ARRAY_OPEN);

  if (scanner.Peek() == Token::ARRAY_CLOSE) {
    scanner.Consume();
    return true;
  }
  
  value = std::make_unique<Value>();
  REQUIRE(value->Parse(scanner));
  children.push_back(std::move(value));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    value = std::make_unique<Value>();
    REQUIRE(value->Parse(scanner));
    children.push_back(std::move(value));
  }

  REQUIRE(scanner.Consume() == Token::ARRAY_CLOSE);

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
  REQUIRE(scanner.Consume() == Token::COLON);

  value = std::make_unique<Value>();
  REQUIRE(value->Parse(scanner));

  children.push_back(std::move(name));
  children.push_back(std::move(value));

  return true;
  
fail:
  return false;
}

bool Value::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    std::unique_ptr<Object> object = std::make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    children.push_back(std::move(object));
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    std::unique_ptr<Array> array = std::make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    children.push_back(std::move(array));
    return true;
  }

  {
    std::unique_ptr<Literal> literal = std::make_unique<Literal>();
    REQUIRE(literal->Parse(scanner));
    children.push_back(std::move(literal));
    return true;
  }

fail:
  return false;
}

bool Literal::Parse(Scanner& scanner) {
  switch (scanner.Consume()) {
  case Token::INT:
    type = Type::INT;
    break;
  case Token::FLOAT:
    type = Type::FLOAT;
    break;
  case Token::STRING:
    type = Type::STRING;
    break;
  case Token::BOOL:
    type = Type::BOOL;
    break;
  case Token::NULLTOKEN:
    type = Type::NULLTYPE;
    break;
  default:
    return false;
  }
  // TODO
  // text =
  return true;
}

bool Name::Parse(Scanner& scanner) {
  // TODO remove this cause it's awful
  auto trim = [](const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v"); // Find the first non-whitespace character
    auto end = str.find_last_not_of(" \t\n\r\f\v");    // Find the last non-whitespace character

    if (start == std::string::npos || end == std::string::npos) {
      return std::string("");
    }
    return str.substr(start, end - start + 1);
  };

  {
    REQUIRE(scanner.Consume() == Token::STRING);
    std::string lexeme = scanner.GetLastLexeme();
    text = trim(scanner.GetLastLexeme());
  }
  return true;

fail:
  return false;
}

/* 'Accept' overrides */

void* AST::Accept(BaseVisitor& visitor)     { return visitor.Visit(*this); }
void* File::Accept(BaseVisitor& visitor)    { return visitor.Visit(*this); }
void* Object::Accept(BaseVisitor& visitor)  { return visitor.Visit(*this); }
void* Array::Accept(BaseVisitor& visitor)   { return visitor.Visit(*this); }
void* Member::Accept(BaseVisitor& visitor)  { return visitor.Visit(*this); }
void* Name::Accept(BaseVisitor& visitor)    { return visitor.Visit(*this); }
void* Value::Accept(BaseVisitor& visitor)   { return visitor.Visit(*this); }
void* Literal::Accept(BaseVisitor& visitor) { return visitor.Visit(*this); }

}
