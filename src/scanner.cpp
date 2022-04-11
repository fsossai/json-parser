#include <string>
#include <memory>
#include <iostream>
#include <stack>
#include <unordered_set>

#include "scanner.h"
#include "token.h"

#define RETURN_TOKEN(x) last_token_ = (x); return last_token_

namespace json_parser {

Scanner::Scanner(const std::string& inStream) {
  input_ = input_.append(inStream);
}

Token Scanner::GetNextToken() {
  while (ValidPos()) {
    // Skip whitespaces
    if (IsWhiteSpace(Char())) {
      ++position_;
      continue;
    }

    switch (Char())
    {
    case ',':
      Validate(1);
      RETURN_TOKEN(Token::COMMA);
    case ':':
      Validate(1);
      RETURN_TOKEN(Token::COLON);
    case '[':
      Validate(1);
      RETURN_TOKEN(Token::ARRAY_OPEN);
    case ']':
      Validate(1);
      RETURN_TOKEN(Token::ARRAY_CLOSE);
    case '{':
      Validate(1);
      RETURN_TOKEN(Token::OBJ_OPEN);
    case '}':
      Validate(1);
      RETURN_TOKEN(Token::OBJ_CLOSE);
    case '"':
      if (Validate(String())) {
        RETURN_TOKEN(Token::STRING);
      }
      RETURN_TOKEN(Token::UNKNOWN);
    default:
      if (IsDigit(Char()) || Char() == '.' || Char() == '-') {
        if (Validate(Integer())) {
          RETURN_TOKEN(Token::INT);
        } 
        if (Validate(Float())) {
          RETURN_TOKEN(Token::FLOAT);
        }
      }
      if (Char() == 't' || Char() == 'f') {
        if (Validate(Boolean())) {
          RETURN_TOKEN(Token::BOOL);
        }
      }
      RETURN_TOKEN(Token::UNKNOWN);
    }
  }
  RETURN_TOKEN(Token::END);
}

Token Scanner::GetLastToken() const {
  return last_token_;
}

bool Scanner::ValidPos(int offset) const {
  return position_ + offset < input_.size();
}

bool Scanner::ValidPos() const {
  return ValidPos(0);
}

void Scanner::PushStatus() {
  saved_pos_.push(position_);
  saved_lexeme_.push(last_lexeme_);
  saved_token_.push(last_token_);
}

void Scanner::RestoreStatus() {
  position_ = saved_pos_.top();
  last_lexeme_ = saved_lexeme_.top();
  last_token_ = saved_token_.top();
}

void Scanner::PopStatus() {
  saved_pos_.pop();
  saved_lexeme_.pop();
  saved_token_.pop();
}

char Scanner::Char(int offset) const {
  return input_[position_ + offset];
}

char Scanner::Char() const {
  return Char(0);
}

int Scanner::String() const {
  int offset = 0;
  if (Char() != '"') {
    return -1;
  }
  ++offset;
  bool skip = false;
  while (position_ + offset < input_.size()) {
    switch (Char(offset))
    {
    case '\\':
      ++offset;
      skip = true;
      break;
    case '"':
      ++offset;
      if (skip) {
        skip = false;
        break;
      }
      return offset;
    default:
      ++offset;
      skip = false;
      break;
    }
  }
  return -1;
}

int Scanner::Integer() const {
  int offset = 0;
  if (ValidPos(offset) && Char(offset) == '-') {
    ++offset;
  }
  while (ValidPos(offset)) {
    if (IsDigit(Char(offset))) {
      ++offset;
    } else {
      if (Char(offset) == '.') {
        return -1;
      }
      return offset;
    }
  }
  return offset;
}

int Scanner::Float() const {
  int offset = 0;
  bool foundDot = false;
  if (ValidPos(offset) && Char(offset) == '-') {
    ++offset;
  }
  while (ValidPos(offset)) {
    if (IsDigit(Char(offset))) {
    } else if (Char(offset) == '.') {
      if (!foundDot) {
        foundDot = true;
      } else {
        break;
      }
    } else {
      break;
    }
    ++offset;
  }
  return offset;
}

int Scanner::Boolean() const {
  if (input_.compare(position_, 4, "true") == 0) {
    return 4;
  }
  if (input_.compare(position_, 5, "false") == 0) {
    return 5;
  }
  return -1;
}

bool Scanner::IsDigit(char c) const {
  return '0' <= c && c <= '9';
}

bool Scanner::IsWhiteSpace(char c) const {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool Scanner::Validate(int length) {
  if (length <= 0) {
    last_lexeme_ = "";
    return false;
  }
  std::size_t newPos = position_ + length;
  if (newPos <= input_.size()) {
    last_lexeme_ = std::string(input_.begin() + position_,
                               input_.begin() + newPos);
    position_ += length;
    return true;
  }
  last_lexeme_ = "";
  std::cerr << "ERROR 0 Scanner::Validate" << std::endl;
  return false;
}

std::string Scanner::GetLastLexeme() const {
  return last_lexeme_;
}

};
