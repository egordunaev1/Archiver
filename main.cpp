#include <string>
#include <ctime>

#include "src/archiver/archiver.h"

int main()
{
    int cur = clock();
    std::vector<std::string> paths = { "folder/mmlbook.pdf"  };
    archiver::zip(paths, "myarch.xuy");
    archiver::unzip("myarch.xuy");
    std::cout << "Time: " << (clock() - cur) / 1000 << "s";
    return 0;
}