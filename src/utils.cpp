#include <iostream>

#include "tokens.h"

namespace JSON {

void PrintToken(Token t) {
  switch (t) {
  case Token::ARRAY_OPEN:
    std::cout << "[";
    break;
  case Token::ARRAY_CLOSE:
    std::cout << "]";
    break;
  case Token::OBJ_OPEN:
    std::cout << "{";
    break;
  case Token::OBJ_CLOSE:
    std::cout << "}";
    break;
  case Token::COMMA:
    std::cout << ",";
    break;
  case Token::COLON:
    std::cout << ":";
    break;
  case Token::UNKNOWN:
    std::cout << "UNKNOWN";
    break;
  case Token::FLOAT:
    std::cout << "FLOAT";
    break;
  case Token::INT:
    std::cout << "INT";
    break;
  case Token::STRING:
    std::cout << "STRING";
    break;
  case Token::END:
    std::cout << "END";
    break;
  default:
    std::cerr << "ERROR 0 ::PrintToken\n";
  }
}

}