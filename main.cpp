#include <string>
#include <ctime>

#include "src/archiver/archiver.h"

int main(int argc, char *argv[])
{
    int cur = static_cast<int>(clock());
    std::vector<std::string> paths = { "folder/mmlbook.pdf"  };
    // archiver::zip(paths, "myarch.xuy");
    archiver::unzip(paths[0]);
    std::cout << "Time: " << (clock() - cur) / 1000 << "s";
    return 0;
}