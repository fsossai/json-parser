#pragma once

#include <string>
#include <memory>
#include <stack>

#include "token.h"

#define REQUIRE(x)  if (!(x)) goto fail;

namespace json_parser {

class Scanner {
public:
  Scanner() = default;
  Scanner(const std::string& inStream);
  Token GetNextToken();
  Token GetLastToken() const;
  std::string GetLastLexeme() const;
  void PushStatus();
  void RestoreStatus();
  void PopStatus();

private:
  std::string input_;
  std::size_t position_ = 0;
  std::string last_lexeme_;
  std::stack<std::size_t> saved_pos_;
  std::stack<std::string> saved_lexeme_;
  std::stack<Token> saved_token_;
  Token last_token_;

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
  bool IsWhiteSpace(char c) const;
  bool Validate(int length);
  
};

}