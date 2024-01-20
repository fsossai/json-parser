#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Scanner.h"

namespace jparser {

template<class T>
bool Match(Scanner& scanner);

template<>
bool Match<File>(Scanner& scanner);

template<>
bool Match<Object>(Scanner& scanner);

template<>
bool Match<Array>(Scanner& scanner);

template<>
bool Match<Member>(Scanner& scanner);

template<>
bool Match<Value>(Scanner& scanner);

template<>
bool Match<Literal>(Scanner& scanner);

template<>
bool Match<Name>(Scanner& scanner);

template<class T>
bool Match(const std::string& input) {
  Scanner scanner(input);
  return Match<T>(scanner);
}

}
