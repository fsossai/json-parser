#include <fstream>
#include <iostream>
#include <sstream>

#include "json_parser.h"

int main(int argc, char **argv) {
  std::stringstream input;
  
  if (argc > 1) {
    std::ifstream file(argv[1]);
    input << file.rdbuf();
  } else {
    input << std::cin.rdbuf();
  }

  json_parser::File file;
  json_parser::Scanner scanner(input.str());

  if (!file.Parse(scanner)) {
    std::cout << "\e[0;31mERROR \e[0m: input text is not in JSON format" << std::endl;
    return 1;
  }

  std::cout << "\e[0;32mOK \e[0m: input text is in JSON format" << std::endl;
  return 0;
}
