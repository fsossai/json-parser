#include <fstream>
#include <iostream>
#include <sstream>

#include "jparser/jparser.h"

using namespace std;
using namespace jparser;

int main(int argc, char **argv) {
  stringstream input;
  
  if (argc > 1) {
    ifstream ifs(argv[1]);
    input << ifs.rdbuf();
  } else {
    input << cin.rdbuf();
  }

  File file;

  if (!file.From(input.str())) {
    cout << "\e[0;31mERROR \e[0m: input text is not in JSON format" << endl;
    return 1;
  }

  cout << file.ToString() << "\n";

  return 0;
}
