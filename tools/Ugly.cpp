#include <fstream>
#include <iostream>
#include <sstream>

#include "JSONParser.h"

using namespace std;

int main(int argc, char **argv) {
  stringstream input;
  
  if (argc > 1) {
    ifstream file(argv[1]);
    input << file.rdbuf();
  } else {
    input << cin.rdbuf();
  }

  json_parser::File file;

  if (!file.From(input.str())) {
    cout << "\e[0;31mERROR \e[0m: input text is not in JSON format" << endl;
    return 1;
  }

  cout << file.ToString() << "\n";

  return 0;
}
