#include <memory>
#include <string>
#include <unordered_set>
#include <iostream>

#include "AST.h"
#include "Scanner.h"
#include "Visitor.h"
#include "StringVisitor.h"

#define REQUIRE(x) if (!(x)) goto fail;

using namespace std;

namespace json_parser {

bool AST::From(const string& input) {
  Scanner scanner(input);
  return Parse(scanner);
}

string AST::ToString() const {
  StringVisitor sv;
  Accept(sv);
  return sv.GetResult();
}

bool File::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    unique_ptr<Object> object = make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    
    file = move(object);
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    unique_ptr<Array> array = make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    
    file = move(array);
    return true;
  }

fail:
  return false;
}

bool Object::Parse(Scanner& scanner) {
  unique_ptr<Member> member;
  unordered_set<string> keys;

  REQUIRE(scanner.Consume() == Token::OBJECT_OPEN);

  if (scanner.Peek() == Token::OBJECT_CLOSE) {
    scanner.Consume();
    return true;
  }

  member = make_unique<Member>();
  REQUIRE(member->Parse(scanner));
  members.push_back(move(member));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    member = make_unique<Member>();
    REQUIRE(member->Parse(scanner));
    auto name_str = member->name->text;
    REQUIRE(keys.find(name_str) == keys.end());
    keys.insert(name_str);
    members.push_back(move(member));
  }

  REQUIRE(scanner.Consume() == Token::OBJECT_CLOSE);
  return true;

fail:
  return false;
}

bool Array::Parse(Scanner& scanner) {
  unique_ptr<Value> value;

  REQUIRE(scanner.Consume() == Token::ARRAY_OPEN);

  if (scanner.Peek() == Token::ARRAY_CLOSE) {
    scanner.Consume();
    return true;
  }
  
  value = make_unique<Value>();
  REQUIRE(value->Parse(scanner));
  values.push_back(move(value));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    value = make_unique<Value>();
    REQUIRE(value->Parse(scanner));
    values.push_back(move(value));
  }

  REQUIRE(scanner.Consume() == Token::ARRAY_CLOSE);

  return true;

fail:
  return false;
}

bool Member::Parse(Scanner& scanner) {
  string name_lexeme;

  name = make_unique<Name>();
  REQUIRE(name->Parse(scanner));
  name = move(name);

  value = make_unique<Value>();
  REQUIRE(scanner.Consume() == Token::COLON);
  REQUIRE(value->Parse(scanner));
  value = move(value);

  return true;
  
fail:
  return false;
}

bool Value::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    unique_ptr<Object> object = make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    value = move(object);
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    unique_ptr<Array> array = make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    value = move(array);
    return true;
  }

  {
    unique_ptr<Literal> literal = make_unique<Literal>();
    REQUIRE(literal->Parse(scanner));
    value = move(literal);
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
  text = scanner.GetLastLiteral();
  return true;
}

bool Name::Parse(Scanner& scanner) {
  REQUIRE(scanner.Consume() == Token::STRING);

  {
    auto name = scanner.GetLastLiteral();
    text = name.substr(1, name.size()-2);
    return true;
  }

fail:
  return false;
}

void* File::Accept(Visitor& visitor)    { return visitor.Visit(*this); }
void* Object::Accept(Visitor& visitor)  { return visitor.Visit(*this); }
void* Array::Accept(Visitor& visitor)   { return visitor.Visit(*this); }
void* Member::Accept(Visitor& visitor)  { return visitor.Visit(*this); }
void* Name::Accept(Visitor& visitor)    { return visitor.Visit(*this); }
void* Value::Accept(Visitor& visitor)   { return visitor.Visit(*this); }
void* Literal::Accept(Visitor& visitor) { return visitor.Visit(*this); }

void* File::Accept(ConstVisitor& visitor)    const { return visitor.Visit(*this); }
void* Object::Accept(ConstVisitor& visitor)  const { return visitor.Visit(*this); }
void* Array::Accept(ConstVisitor& visitor)   const { return visitor.Visit(*this); }
void* Member::Accept(ConstVisitor& visitor)  const { return visitor.Visit(*this); }
void* Name::Accept(ConstVisitor& visitor)    const { return visitor.Visit(*this); }
void* Value::Accept(ConstVisitor& visitor)   const { return visitor.Visit(*this); }
void* Literal::Accept(ConstVisitor& visitor) const { return visitor.Visit(*this); }
}
