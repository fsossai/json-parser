#include <memory>
#include <string>

#include "parser.h"
#include "scanner.h"

namespace json {

Parser::Parser(Scanner& scanner)
    : scanner_(scanner) {
  Reparse();
}

void Parser::Reparse() {
  file_ = std::make_unique<File>();
  file_->Parse(scanner_);
}

void* Parser::Process(BaseVisitor& visitor) {
  return file_->Accept(visitor);
}

void* BaseVisitor::Visit(NonTerminal&) { return nullptr; }
void* BaseVisitor::Visit(File&) { return nullptr; }
void* BaseVisitor::Visit(Object&) { return nullptr; }
void* BaseVisitor::Visit(Array&) { return nullptr; }
void* BaseVisitor::Visit(Member&) { return nullptr; }
void* BaseVisitor::Visit(Name&) { return nullptr; }
void* BaseVisitor::Visit(Value&) { return nullptr; }
void* BaseVisitor::Visit(Literal&) { return nullptr; }

void* NonTerminal::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

std::string NonTerminal::ToString() const {
  return str_;
}

bool File::Parse(Scanner& scanner) {
  Token tok;
  std::unique_ptr<Object> object = std::make_unique<Object>();
  std::unique_ptr<Array> array = std::make_unique<Array>();
  scanner.PushStatus();
  if (object->Parse(scanner)) {
    tok = scanner.GetNextToken();
    if (tok == Token::END) {
      str_ = object->ToString();
      children.push_back(std::move(object));
      scanner.PopStatus();
      return true;
    } else {
      goto fail;
    }
  }
  scanner.RestoreStatus();
  if (array->Parse(scanner)) {
    tok = scanner.GetNextToken();
    if (tok == Token::END) {
      str_ = array->ToString();
      children.push_back(std::move(array));
      scanner.PopStatus();
      return true;
    }
    else {
      goto fail;
    }
  }
  fail:
  scanner.RestoreStatus();
  return false;
}

bool Object::Parse(Scanner& scanner) {
  scanner.PushStatus();
  std::string tmp;
  Token tok = scanner.GetNextToken();
  if (tok == Token::OBJ_OPEN) {
    str_ += scanner.GetLastLexeme();
    std::unique_ptr<Member> member = std::make_unique<Member>();
    if (member->Parse(scanner)) {
      str_ += member->ToString();
      children.push_back(std::move(member));
      tok = scanner.GetNextToken();
      while (tok == Token::COMMA) {
        str_ += scanner.GetLastLexeme();
        member = std::make_unique<Member>();
        if (member->Parse(scanner)) {
          str_ += member->ToString();
          children.push_back(std::move(member));
        } else {
          goto fail;
        }
        tok = scanner.GetNextToken();
      }
      if (tok == Token::OBJ_CLOSE) {
        str_ += scanner.GetLastLexeme();
        scanner.PopStatus();
        return true;
      }
    }
  }
  fail:
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Array::Parse(Scanner& scanner) {
  scanner.PushStatus();
  Token tok = scanner.GetNextToken();
  if (tok == Token::ARRAY_OPEN) {
    str_ += scanner.GetLastLexeme();
    std::unique_ptr<Value> value = std::make_unique<Value>();
    if (value->Parse(scanner)) {
      str_ += value->ToString();
      children.push_back(std::move(value));
      tok = scanner.GetNextToken();
      while (tok == Token::COMMA) {
        str_ += scanner.GetLastLexeme();
        value = std::make_unique<Value>();
        if (value->Parse(scanner)) {
          str_ += value->ToString();
          children.push_back(std::move(value));
        } else {
          goto fail;
        }
        tok = scanner.GetNextToken();
      }
      if (tok == Token::ARRAY_CLOSE) {
        str_ += scanner.GetLastLexeme();
        scanner.PopStatus();
        return true;
      }
    }
  }
  fail:
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Member::Parse(Scanner& scanner) {
  scanner.PushStatus();
  std::unique_ptr<Name> name = std::make_unique<Name>();
  if (name->Parse(scanner)) {
    std::string name_lexeme = scanner.GetLastLexeme();
    Token tok = scanner.GetNextToken();
    if (tok == Token::COLON) {
      std::unique_ptr<Value> value = std::make_unique<Value>();
      if (value->Parse(scanner)) {
        str_ = name_lexeme + ": " + value->ToString();
        children.push_back(std::move(name));
        children.push_back(std::move(value));
        scanner.PopStatus();
        return true;
      }
    }
  }
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

bool Value::Parse(Scanner& scanner) {
  std::unique_ptr<Object> object = std::make_unique<Object>();
  std::unique_ptr<Array> array = std::make_unique<Array>();
  std::unique_ptr<Literal> literal = std::make_unique<Literal>();

  scanner.PushStatus();
  if (object->Parse(scanner)) {
    str_ = object->ToString();
    children.push_back(std::move(object));
    scanner.PopStatus();
    return true;
  }
  scanner.RestoreStatus();
  if (array->Parse(scanner)) {
    str_ = array->ToString();
    children.push_back(std::move(array));
    scanner.PopStatus();
    return true;
  }
  scanner.RestoreStatus();
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
  std::string lexeme = scanner.GetLastLexeme();
  if (tok == Token::INT) {
    IntData = std::stoi(lexeme);
    data = &IntData;
    type_ = Type::INT;
  } else if (tok == Token::FLOAT) {
    FloatData = std::stof(lexeme);
    data = &FloatData;
    type_ = Type::FLOAT;
  } else if (tok == Token::STRING) {
    StringData = lexeme;
    data = &StringData;
    type_ = Type::STRING;
  } else {
    scanner.RestoreStatus();
    scanner.PopStatus();
    return false;
  }
  str_ = lexeme;
  scanner.PopStatus();
  return true;
}

bool Name::Parse(Scanner& scanner) {
  scanner.PushStatus();
  Token tok = scanner.GetNextToken();
  std::string name_lexeme = scanner.GetLastLexeme();
  if (tok == Token::STRING) {
    str_ = name = name_lexeme;
    scanner.PopStatus();
    return true;
  }
  scanner.RestoreStatus();
  scanner.PopStatus();
  return false;
}

Literal::Type Literal::GetType() const {
  return type_;
}

/* 'Accept' overrides */

void* File::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

void* Object::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

void* Array::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

void* Member::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

void* Name::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

void* Value::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

void* Literal::Accept(BaseVisitor& visitor) {
  return visitor.Visit(*this);
}

}
