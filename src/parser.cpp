#include <memory>
#include <string>
#include <stdio.h> //D

#include "parser.h"
#include "scanner.h"

namespace JSON {

void* BaseVisitor::Visit(NonTerminal& /*nt*/) {
  return nullptr;
}

NonTerminal::NonTerminal(Scanner& scanner)
    : scanner_(scanner) {
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

bool File::Parse() {
  Token tok;
  Object object(scanner_);
  Array array(scanner_);
  scanner_.PushStatus();
  if (object.Parse()) {
    tok = scanner_.GetNextToken();
    if (tok == Token::END) {
      str_ = object.ToString();
      return true;
    } else {
      goto fail;
    }
  }
  scanner_.PopStatus();
  scanner_.PushStatus();
  if (array.Parse()) {
    tok = scanner_.GetNextToken();
    if (tok == Token::END) {
      str_ = array.ToString();
      return true;
    }
    else {
      goto fail;
    }
  }
  fail:
  scanner_.PopStatus();
  return false;
}

bool Object::Parse() {
  scanner_.PushStatus();
  std::string tmp;
  Token tok = scanner_.GetNextToken();
  if (tok == Token::OBJ_OPEN) {
    str_ += scanner_.GetLastLexeme();
    Member member(scanner_);
    if (member.Parse()) {
      str_ += member.ToString();
      tok = scanner_.GetNextToken();
      while (tok == Token::COMMA) {
        str_ += scanner_.GetLastLexeme();
        if (member.Parse()) {
          str_ += member.ToString();
        } else {
          goto fail;
        }
        tok = scanner_.GetNextToken();
      }
      if (tok == Token::OBJ_CLOSE) {
        str_ += scanner_.GetLastLexeme();
        return true;
      }
    }
  }
  fail:
  scanner_.PopStatus();
  return false;
}

bool Array::Parse() {
  scanner_.PushStatus();
  std::string tmp;
  Token tok = scanner_.GetNextToken();
  if (tok == Token::ARRAY_OPEN) {
    str_ += scanner_.GetLastLexeme();
    Value value(scanner_);
    if (value.Parse()) {
      str_ += value.ToString();
      tok = scanner_.GetNextToken();
      while (tok == Token::COMMA) {
        str_ += scanner_.GetLastLexeme();
        if (value.Parse()) {
          str_ += value.ToString();
        } else {
          goto fail;
        }
        tok = scanner_.GetNextToken();
      }
      if (tok == Token::ARRAY_CLOSE) {
        str_ += scanner_.GetLastLexeme();
        return true;
      }
    }
  }
  fail:
  scanner_.PopStatus();
  return false;
}

bool Member::Parse() {
  scanner_.PushStatus();
  Token tok = scanner_.GetNextToken();
  if (tok == Token::STRING) {
    std::string str = scanner_.GetLastLexeme();
    tok = scanner_.GetNextToken();
    if (tok == Token::COLON) {
      Value value(scanner_);
      if (value.Parse()) {
        str_ = str + ": " + value.ToString();
        return true;
      }
    }
  }
  scanner_.PopStatus();
  return false;
}

bool Value::Parse() {
  Object object(scanner_);
  Array array(scanner_);
  Literal literal(scanner_);

  scanner_.PushStatus();
  if (object.Parse()) {
    str_ = object.ToString();
    return true;
  }
  scanner_.PopStatus();
  scanner_.PushStatus();
  if (array.Parse()) {
    str_ = array.ToString();
    return true;
  }
  scanner_.PopStatus();
  scanner_.PushStatus();
  if (literal.Parse()) {
    str_ = literal.ToString();
    return true;
  }
  scanner_.PopStatus();
  return false;
}

bool Literal::Parse() {
  Token tok = scanner_.GetNextToken();
  std::string lexeme = scanner_.GetLastLexeme();
  this->str_ = lexeme;
  scanner_.PushStatus();
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
    scanner_.PopStatus();
    return false;
  }
  return true;
}

Literal::Type Literal::GetType() const {
  return type_;
}

}
