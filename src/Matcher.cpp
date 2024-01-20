#include <memory>
#include <string>
#include <unordered_set>
#include <iostream>

#include "jparser/AST.h"
#include "jparser/Matcher.h"

#define REQUIRE(x) if (!(x)) goto fail;

using namespace std;

namespace jparser {

template<>
bool Match<File>(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    REQUIRE(Match<Object>(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    REQUIRE(Match<Array>(scanner));
    REQUIRE(scanner.Consume() == Token::END);
    return true;
  }

fail:
  return false;
}

template<>
bool Match<Object>(Scanner& scanner) {
  unordered_set<string> keys;

  REQUIRE(scanner.Consume() == Token::OBJECT_OPEN);

  if (scanner.Peek() == Token::OBJECT_CLOSE) {
    scanner.Consume();
    return true;
  }

  REQUIRE(Match<Member>(scanner));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    Member member;
    REQUIRE(member.Parse(scanner));
    auto name_str = member.name->text;
    REQUIRE(keys.find(name_str) == keys.end());
    keys.insert(name_str);
  }

  REQUIRE(scanner.Consume() == Token::OBJECT_CLOSE);
  return true;

fail:
  return false;
}

template<>
bool Match<Array>(Scanner& scanner) {
  REQUIRE(scanner.Consume() == Token::ARRAY_OPEN);

  if (scanner.Peek() == Token::ARRAY_CLOSE) {
    scanner.Consume();
    return true;
  }
  
  REQUIRE(Match<Value>(scanner));

  while (scanner.Peek() == Token::COMMA) {
    scanner.Consume();
    REQUIRE(Match<Value>(scanner));
  }

  REQUIRE(scanner.Consume() == Token::ARRAY_CLOSE);
  return true;

fail:
  return false;
}

template<>
bool Match<Member>(Scanner& scanner) {
  REQUIRE(Match<Name>(scanner));
  REQUIRE(scanner.Consume() == Token::COLON);
  REQUIRE(Match<Value>(scanner));
  return true;
  
fail:
  return false;
}

template<>
bool Match<Value>(Scanner& scanner) {
  if (scanner.Peek() == Token::OBJECT_OPEN) {
    REQUIRE(Match<Object>(scanner));
    return true;
  }

  if (scanner.Peek() == Token::ARRAY_OPEN) {
    REQUIRE(Match<Array>(scanner));
    return true;
  }

  REQUIRE(Match<Literal>(scanner));
  return true;

fail:
  return false;
}

template<>
bool Match<Literal>(Scanner& scanner) {
  switch (scanner.Consume()) {
  case Token::INT:
  case Token::FLOAT:
  case Token::STRING:
  case Token::BOOL:
  case Token::NULLTOKEN:
    break;
  default:
    return false;
  }
  return true;
}

template<>
bool Match<Name>(Scanner& scanner) {
  REQUIRE(scanner.Consume() == Token::STRING);
  return true;

fail:
  return false;
}

}
