#pragma once

#include <string>
#include <memory>

#include "tokens.h"

namespace JSON {

class Scanner {
public:
  Scanner(const std::string& inStream);
  bool AddInputStream(const std::string& inStream);
  Token GetNextToken();

private:
  std::string inStream_;
  std::size_t position_ = 0;
  std::string last_lexeme_;

  bool ValidPos(int offset) const;
  bool ValidPos() const;
  char Char(int offset) const;
  char Char() const;
  int String() const;
  int Integer() const;
  int Float() const;
  bool IsDigit(char c) const;
  bool IsWhiteSpace(char c) const;
  bool Validate(int length);
  std::string GetLastLexeme();
};

}