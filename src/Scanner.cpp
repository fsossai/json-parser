#include <string>
#include <memory>
#include <iostream>
#include <stack>

#include "Scanner.h"

#define REQUIRE(x) if (!(x)) goto fail;

using namespace std;

namespace json_parser {

Scanner::Scanner(const string& input)
    : input_(input)
    , position_(0)
    , scanned_(false) {
}

Token Scanner::Scan(bool consume) {
#define RETURN_TOKEN(x) \
  last_token_ = (x); \
  last_buffer_ = string(input_.begin() + start_position, \
                             input_.begin() + position_); \
  if (consume) { \
    scanned_ = false; \
  } else { \
    position_ = start_position; \
    scanned_ = true; \
  } \
  return last_token_

  if (scanned_) {
    if (consume) {
      scanned_ = false;
      position_ += last_buffer_.size();
    }
    return last_token_;
  }

  size_t start_position = position_;

  while (ValidPos()) {
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
      RETURN_TOKEN(Token::OBJECT_OPEN);
    case '}':
      Validate(1);
      RETURN_TOKEN(Token::OBJECT_CLOSE);
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
      if (Char() == 'n') {
        if (Validate(Null())) {
          RETURN_TOKEN(Token::NULLTOKEN);
        }
      }
      RETURN_TOKEN(Token::UNKNOWN);
    }
  }
  RETURN_TOKEN(Token::END);

#undef RETURN_TOKEN
}

Token Scanner::Peek() {
  return Scan(/*consume=*/false);
}

Token Scanner::Consume() {
  return Scan(/*consume=*/true);
}

Token Scanner::GetLastToken() const {
  return last_token_;
}

string Scanner::GetLastLiteral() const {
  return last_literal_;
}
string Scanner::GetLastBuffer() const {
  return last_buffer_;
}

bool Scanner::ValidPos(int offset) const {
  return position_ + offset < input_.size();
}

bool Scanner::ValidPos() const {
  return ValidPos(0);
}

char Scanner::Char(int offset) const {
  return input_[position_ + offset];
}

char Scanner::Char() const {
  return Char(0);
}

int Scanner::String() {
  int offset = 0;
  if (Char() != '"') {
    return -1;
  }
  ++offset;
  bool escape = false;
  while (ValidPos(offset)) {
    switch (Char(offset))
    {
    case '\\':
      ++offset;
      escape = !escape;
      break;
    case '\n':
    case '\r':
    case '\t':
      return -1;
    case '"':
      ++offset;
      if (escape) {
        escape = false;
        break;
      }
      last_literal_ = input_.substr(position_, offset);
      return offset;
    default:
      if (escape) {
        REQUIRE(Char(offset) == 'b' || Char(offset) == 'f' ||
                Char(offset) == 'n' || Char(offset) == 'r' ||
                Char(offset) == 't' || Char(offset) == 'u' ||
                Char(offset) == '/');
        if (Char(offset) == 'u') {
          for (int i = 0; i < 4; i++) {
            ++offset;
            REQUIRE(ValidPos(offset));
            REQUIRE(IsHex(Char(offset)));
          }
        }
      }
      ++offset;
      escape = false;
      break;
    }
  }

  fail:
  return -1;
}

int Scanner::Integer() {
  int offset = 0;
  REQUIRE(ValidPos(offset));
  if (Char(offset) == '-') {
    ++offset;
    REQUIRE(ValidPos(offset));
    REQUIRE(IsDigit(Char(offset)));
  }
  if (Char(offset) == '0') {
    ++offset;
    if (ValidPos(offset)) {
      REQUIRE(!IsDigit(Char(offset)));
      if (Char(offset) != '.') {
        last_literal_ = input_.substr(position_, offset);
        return offset;
      }
    }
  }
  while (ValidPos(offset)) {
    if (IsDigit(Char(offset))) {
      ++offset;
    } else {
      REQUIRE(Char(offset) != '.');
      REQUIRE(Char(offset) != 'e');
      REQUIRE(Char(offset) != 'E');
      last_literal_ = input_.substr(position_, offset);
      return offset;
    }
  }
  last_literal_ = input_.substr(position_, offset);
  return offset;
  
  fail:
  return -1;
}

int Scanner::Float() {
  int offset = 0;
  bool foundDot = false;
  bool foundE = false;
  REQUIRE(ValidPos(offset));
  if (Char(offset) == '-') {
    ++offset;
    REQUIRE(IsDigit(Char(offset)));
  }
  if (Char(offset) == '0') {
    ++offset;
    if (ValidPos(offset)) {
      REQUIRE(!IsDigit(Char(offset)));
      if (Char(offset) != '.') {
        last_literal_ = input_.substr(position_, offset);
        return offset;
      }
    }
  }
  while (ValidPos(offset)) {
    if (IsDigit(Char(offset))) {
    } else if (Char(offset) == '.') {
      REQUIRE(!foundDot);
      foundDot = true;
      ++offset;
      REQUIRE(ValidPos(offset));
      REQUIRE(IsDigit(Char(offset)));
    } else if (Char(offset) == 'e' || Char(offset) == 'E') {
      REQUIRE(!foundE);
      foundE = true;
      ++offset;
      REQUIRE(ValidPos(offset));
      if (!IsDigit(Char(offset))) {
        REQUIRE(Char(offset) == '+' ||
                Char(offset) == '-');
        ++offset;
        REQUIRE(ValidPos(offset));
        REQUIRE(IsDigit(Char(offset)));
      }
    } else {
      break;
    }
    ++offset;
  }
  last_literal_ = input_.substr(position_, offset);
  return offset;

  fail:
  return -1;
}

int Scanner::Boolean() {
  if (input_.compare(position_, 4, "true") == 0) {
    last_literal_ = "true";
    return 4;
  }
  if (input_.compare(position_, 5, "false") == 0) {
    last_literal_ = "false";
    return 5;
  }
  return -1;
}

int Scanner::Null() {
  if (input_.compare(position_, 4, "null") == 0) {
    last_literal_ = "null";
    return 4;
  }
  return -1;
}

bool Scanner::IsDigit(char c) const {
  return '0' <= c && c <= '9';
}

bool Scanner::IsHex(char c) const {
  return ('0' <= c && c <= '9') ||
         ('a' <= c && c <= 'f') ||
         ('A' <= c && c <= 'F');
}

bool Scanner::IsWhiteSpace(char c) const {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool Scanner::Validate(int length) {
  if (length <= 0) {
    return false;
  }
  position_ += length;
  return true;
}

string TokenToString(Token token) {
  switch (token) {
  case Token::ARRAY_OPEN:   return "[";
  case Token::ARRAY_CLOSE:  return "]";
  case Token::OBJECT_OPEN:     return "{";
  case Token::OBJECT_CLOSE:    return "}";
  case Token::COMMA:        return ",";
  case Token::COLON:        return ":";
  case Token::UNKNOWN:      return "UNKNOWN";
  case Token::FLOAT:        return "FLOAT";
  case Token::INT:          return "INT";
  case Token::STRING:       return "STRING";
  case Token::END:          return "END";
  default:                  return "";
  }
}

};
