#include <string>
#include <ctime>

#include "src/archiver/archiver.h"

int main()
{
    int cur = clock();
    std::vector<std::string> paths = { "folder/x.mp4", "folder/y.mp4", "folder/z.PDF"  };
    archiver::zip(paths, "myarch.xuy");
    archiver::unzip("myarch.xuy");
    std::cout << "Время работы " << cur - clock();
    return 0;
}