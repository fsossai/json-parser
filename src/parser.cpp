#include <memory>
#include <string>
#include <stdio.h> //D

#include "parser.h"
#include "scanner.h"

namespace JSON {

void* BaseVisitor::Visit(NonTerminal& /*nt*/) {
  return nullptr;
}

void* NonTerminal::Accept(BaseVisitor visitor) {
  return visitor.Visit(*this);
}

std::string NonTerminal::ToString() const {
  return str_;
}

Parser::Parser(Scanner& scanner) 
    : scanner_(scanner) {
}

bool File::Parse(Scanner& scanner) {
  Token tok;
  Object object;
  Array array;
  scanner.PushStatus();
  if (object.Parse(scanner)) {
    tok = scanner.GetNextToken();
    if (tok == Token::END) {
      str_ = object.ToString();
      return true;
    } else {
      goto fail;
    }
  }
  scanner.PopStatus();
  scanner.PushStatus();
  if (array.Parse(scanner)) {
    tok = scanner.GetNextToken();
    if (tok == Token::END) {
      str_ = array.ToString();
      return true;
    }
    else {
      goto fail;
    }
  }
  fail:
  scanner.PopStatus();
  return false;
}

bool Object::Parse(Scanner& scanner) {
  scanner.PushStatus();
  std::string tmp;
  Token tok = scanner.GetNextToken();
  if (tok == Token::OBJ_OPEN) {
    str_ += scanner.GetLastLexeme();
    Member member;
    if (member.Parse(scanner)) {
      str_ += member.ToString();
      tok = scanner.GetNextToken();
      while (tok == Token::COMMA) {
        str_ += scanner.GetLastLexeme();
        if (member.Parse(scanner)) {
          str_ += member.ToString();
        } else {
          goto fail;
        }
        tok = scanner.GetNextToken();
      }
      if (tok == Token::OBJ_CLOSE) {
        str_ += scanner.GetLastLexeme();
        return true;
      }
    }
  }
  fail:
  scanner.PopStatus();
  return false;
}

bool Array::Parse(Scanner& scanner) {
  scanner.PushStatus();
  std::string tmp;
  Token tok = scanner.GetNextToken();
  if (tok == Token::ARRAY_OPEN) {
    str_ += scanner.GetLastLexeme();
    Value value;
    if (value.Parse(scanner)) {
      str_ += value.ToString();
      tok = scanner.GetNextToken();
      while (tok == Token::COMMA) {
        str_ += scanner.GetLastLexeme();
        if (value.Parse(scanner)) {
          str_ += value.ToString();
        } else {
          goto fail;
        }
        tok = scanner.GetNextToken();
      }
      if (tok == Token::ARRAY_CLOSE) {
        str_ += scanner.GetLastLexeme();
        return true;
      }
    }
  }
  fail:
  scanner.PopStatus();
  return false;
}

bool Member::Parse(Scanner& scanner) {
  scanner.PushStatus();
  Token tok = scanner.GetNextToken();
  if (tok == Token::STRING) {
    std::string str = scanner.GetLastLexeme();
    tok = scanner.GetNextToken();
    if (tok == Token::COLON) {
      Value value;
      if (value.Parse(scanner)) {
        str_ = str + ": " + value.ToString();
        return true;
      }
    }
  }
  scanner.PopStatus();
  return false;
}

bool Value::Parse(Scanner& scanner) {
  Object object;
  Array array;
  Literal literal;

  scanner.PushStatus();
  if (object.Parse(scanner)) {
    str_ = object.ToString();
    return true;
  }
  scanner.PopStatus();
  scanner.PushStatus();
  if (array.Parse(scanner)) {
    str_ = array.ToString();
    return true;
  }
  scanner.PopStatus();
  scanner.PushStatus();
  if (literal.Parse(scanner)) {
    str_ = literal.ToString();
    return true;
  }
  scanner.PopStatus();
  return false;
}

bool Literal::Parse(Scanner& scanner) {
  Token tok = scanner.GetNextToken();
  std::string lexeme = scanner.GetLastLexeme();
  this->str_ = lexeme;
  scanner.PushStatus();
  if (tok == Token::INT) {
    IntData = std::stoi(lexeme);
    type_ = Type::INT;
  } else if (tok == Token::FLOAT) {
    FloatData = std::stof(lexeme);
    type_ = Type::FLOAT;
  } else if (tok == Token::STRING) {
    StringData = lexeme;
    type_ = Type::STRING;
  } else {
    scanner.PopStatus();
    return false;
  }
  return true;
}

Literal::Type Literal::GetType() const {
  return type_;
}

}
