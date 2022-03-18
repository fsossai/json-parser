#pragma once

#include <memory>

#include "scanner.h"

namespace JSON {

class Parser {
public:
  Parser();

private:
  std::shared_ptr<Scanner> scanner_;
};

}
