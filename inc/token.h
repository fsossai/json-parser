#pragma once

#include <string>

namespace json_parser {

enum class Token {
  ARRAY_OPEN,
  ARRAY_CLOSE,
  OBJ_OPEN,
  OBJ_CLOSE,
  COMMA,
  COLON,
  STRING,
  INT,
  FLOAT,
  BOOL,
  UNKNOWN,
  END
};

std::string TokenToString(Token t);

}