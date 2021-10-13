#include <string>
#include "src/archiver/archiver.h"

int main()
{
    std::vector<std::string> paths = { "folder/a.txt", "folder/b.txt" };
    archiver::zip(paths, "myarch.xuy");
    archiver::unzip("myarch.xuy");
    return 0;
}