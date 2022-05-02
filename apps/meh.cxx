#include <iostream>
#include <meh/logic.hpp>

using namespace meh;

int main(int argc, char *argv[]) {

  // Loop through files specified as inputs and add them to the 3MF wrapper
  if (argc > 2) {
    std::cout << "Will export file to " << argv[1] << std::endl;
    char *out;
    out = argv[1];

    char *in[argc - 2];
    for (int i = 2; i < argc; i++) {
      std::cout << "Adding " << argv[i] << " to process list" << std::endl;
      in[i - 2] = argv[i];
    }
    exportModelListTo3MF(in, out, argc-2);
  } else {
    std::cout << "No model files where specified";
  }

  std::cout << "Done" << std::endl;
}
