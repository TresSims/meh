#include "MehViewer.hpp"

using namespace pmp;

int main(int argc, char **argv)
{
        MehViewer window("Meh", 800, 800);

        if(argc > 1)
                window.load_mesh(argv[1]);

        return window.run();
}
