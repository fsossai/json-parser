#include <fstream>
#include <iostream>
#include <sstream>

#include "JSONParser.h"

using namespace std;
using namespace json_parser;

int main(int argc, char **argv) {
  stringstream input;
  
  if (argc > 1) {
    ifstream file(argv[1]);
    input << file.rdbuf();
  } else {
    input << cin.rdbuf();
  }

  if (!Match<File>(input.str())) {
    cout << "\e[0;31mERROR \e[0m: input text is not in JSON format" << endl;
    return 1;
  }

  cout << "\e[0;32mOK \e[0m: input text is in JSON format" << endl;
  return 0;
}
