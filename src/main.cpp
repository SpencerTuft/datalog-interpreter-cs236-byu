#include <iostream>
#include "./Interpreter.h"

/**
 * Main process
 * @param argc = argument count
 * @param argv = argument vector
 * @return
 */
int main(int argc, const char *argv[]) {
  std::string fileName = argv[1];

  if (argc < 1) {
    std::cout << "Missing input file argument" << std::endl;
    return 0;
  }
  std::string inputFileName = std::string(fileName);
  Interpreter interpreter(inputFileName);

  auto db = interpreter.build();

  auto queries = interpreter.getQueries();
  auto queryResults = db.execute(queries);

  for (int i = 0, max = static_cast<int>(queryResults.size()); i < max; i++) {
    if (i != 0) std::cout << std::endl;
    std::cout << queries[i].str() << " ";
    int size = queryResults[i].size();
    if (size == 0) {
      std::cout << "No";
    } else {
      std::cout << "Yes(" << size << ")";
    }
    if (!queryResults[i].undefined() && !queryResults[i].empty()) {
      std::cout << std::endl << queryResults[i].str();
    }
  }

  return 0;
}