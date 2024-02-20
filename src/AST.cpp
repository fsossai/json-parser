#include <memory>
#include <string>
#include <unordered_set>
#include <iostream>

#include "jparser/AST.h"
#include "jparser/Scanner.h"
#include "jparser/StringVisitor.h"
#include "jparser/Visitor.h"

#define REQUIRE(x) if (!(x)) goto fail;

using namespace std;

namespace jparser {

bool AST::From(const string& input) {
  Scanner scanner(input);
  return Parse(scanner);
}

string AST::ToString() const {
  StringVisitor sv;
  Accept(sv);
  return sv.GetResult();
}

bool Document::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    unique_ptr<Object> object = make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    
    document = std::move(object);
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    unique_ptr<Array> array = make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    
    document = std::move(array);
    return true;
  }

fail:
  return false;
}

bool StreamDocument::Parse(Scanner& scanner) {
  unique_ptr<Value> value;

  value = make_unique<Value>();
  REQUIRE(value->Parse(scanner));
  values.push_back(std::move(value));

  while (scanner.Peek() != Token::END) {
    value = make_unique<Value>();
    REQUIRE(value->Parse(scanner));
    values.push_back(std::move(value));
  }

  return true;

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
  valueMap[member->name->text] = member->value.get();
  members.push_back(std::move(member));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    member = make_unique<Member>();
    REQUIRE(member->Parse(scanner));
    auto name_str = member->name->text;
    REQUIRE(keys.find(name_str) == keys.end());
    keys.insert(name_str);
    valueMap[name_str] = member->value.get();
    members.push_back(std::move(member));
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
  values.push_back(std::move(value));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    value = make_unique<Value>();
    REQUIRE(value->Parse(scanner));
    values.push_back(std::move(value));
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
  name = std::move(name);

  value = make_unique<Value>();
  REQUIRE(scanner.Consume() == Token::COLON);
  REQUIRE(value->Parse(scanner));
  value = std::move(value);

  return true;
  
fail:
  return false;
}

bool Value::Parse(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    unique_ptr<Object> object = make_unique<Object>();
    REQUIRE(object->Parse(scanner));
    value = std::move(object);
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    unique_ptr<Array> array = make_unique<Array>();
    REQUIRE(array->Parse(scanner));
    value = std::move(array);
    return true;
  }

  {
    unique_ptr<Literal> literal = make_unique<Literal>();
    REQUIRE(literal->Parse(scanner));
    value = std::move(literal);
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

void *Document::Accept(Visitor& v) { return v.Visit(*this); }
void *StreamDocument::Accept(Visitor& v) { return v.Visit(*this); }
void *Object::Accept(Visitor& v) { return v.Visit(*this); }
void *Array::Accept(Visitor& v) { return v.Visit(*this); }
void *Member::Accept(Visitor& v) { return v.Visit(*this); }
void *Name::Accept(Visitor& v) { return v.Visit(*this); }
void *Value::Accept(Visitor& v) { return v.Visit(*this); }
void *Literal::Accept(Visitor& v) { return v.Visit(*this); }

void *Document::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *StreamDocument::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *Object::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *Array::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *Member::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *Name::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *Value::Accept(ConstVisitor& v) const { return v.Visit(*this); }
void *Literal::Accept(ConstVisitor& v) const { return v.Visit(*this); }

}
