#include <string>

#include "token.h"

namespace json_parser {

std::string TokenToString(Token t) {
  switch (t) {
  case Token::ARRAY_OPEN:   return "[";
  case Token::ARRAY_CLOSE:  return "]";
  case Token::OBJ_OPEN:     return "{";
  case Token::OBJ_CLOSE:    return "}";
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

}