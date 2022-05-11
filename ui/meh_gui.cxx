#include "MehViewer.hpp"

using namespace pmp;

int main(int argc, char **argv) {
  MehViewer window("Meh", 800, 800);

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      window.load_mesh(argv[i]);
    }
  }

  return window.run();
}
