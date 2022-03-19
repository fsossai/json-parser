#include <string>
#include <memory>
#include <iostream>
#include <stack>

#include "tokens.h"
#include "scanner.h"

namespace JSON {

Scanner::Scanner(const std::string& inStream) {
  AddInputStream(inStream);
}
  
bool Scanner::AddInputStream(const std::string& inStream) {
  inStream_ = inStream_.append(inStream);
  return true;
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
      return Token::COMMA;
    case ':':
      Validate(1);
      return Token::COLON;
    case '[':
      Validate(1);
      return Token::ARRAY_OPEN;
    case ']':
      Validate(1);
      return Token::ARRAY_CLOSE;
    case '{':
      Validate(1);
      return Token::OBJ_OPEN;
    case '}':
      Validate(1);
      return Token::OBJ_CLOSE;
    case '"':
      if (Validate(String())) {
        return Token::STRING;
      }
      return Token::UNKNOWN;
    default:
      if (IsDigit(Char()) || Char() == '.') {
        if (Validate(Integer())) {
          return Token::INT;
        } 
        if (Validate(Float())) {
          return Token::FLOAT;
        }
      }
      return Token::UNKNOWN;
    }
  }
  return Token::END;
}

bool Scanner::ValidPos(int offset) const {
  return position_ + offset < inStream_.size();
}

bool Scanner::ValidPos() const {
  return ValidPos(0);
}

void Scanner::PushStatus() {
  saved_pos_.push(position_);
  saved_lexeme_.push(last_lexeme_);
}

void Scanner::PopStatus() {
  position_ = saved_pos_.top();
  last_lexeme_ = saved_lexeme_.top();
  saved_pos_.pop();
  saved_lexeme_.pop();
}

char Scanner::Char(int offset) const {
  return inStream_[position_ + offset];
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
  while (position_ + offset < inStream_.size()) {
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
  if (newPos <= inStream_.size()) {
    last_lexeme_ = std::string(inStream_.begin() + position_,
                               inStream_.begin() + newPos);
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
