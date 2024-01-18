#pragma once

#include <string>
#include <memory>
#include <stack>

#include "token.h"

namespace json_parser {

class Scanner {
public:
  Scanner() = default;
  Scanner(const std::string& input);
  Token Peek();
  Token Consume();
  Token GetLastToken() const;
  std::string GetLastLexeme() const;

private:
  std::string input_;
  std::size_t position_;
  std::string last_lexeme_;
  std::string last_offset_;
  Token last_token_;
  bool scanned_;

  Token Scan(bool consume);
  bool ValidPos(int offset) const;
  bool ValidPos() const;
  char Char(int offset) const;
  char Char() const;
  int String() const;
  int Integer() const;
  int Float() const;
  int Boolean() const;
  int Null() const;
  bool IsDigit(char c) const;
  bool IsHex(char c) const;
  bool IsWhiteSpace(char c) const;
  bool Validate(int length);
  
};

}
