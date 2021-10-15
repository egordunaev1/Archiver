#include <string>
#include <ctime>
#include <fstream>
#include <vector>

#include "src/archiver/archiver.h"

bool check_file(char file[]) {
    return !std::fstream(file).fail();
}

int main(int argc, char *argv[])
{
    if (argc == 1 || (argc > 1 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "-help"))) {
        std::string help = "================================================================\n"
                           "||                           ARCHIVER                         ||\n"
                           "================================================================\n"
                           "|--------------------------------------------------------------| \n"
                           "| -h                                     ==> HELP              |\n"
                           "| zip <archive_name> <file1> <file2> ... ==> ZIP FILES         |\n"
                           "| unzip <archive_name>                   ==> UNZIP AN ARCHIVE  |\n"
                           "|--------------------------------------------------------------|";
        std::cout << help;
    } else if (std::string(argv[1]) == "zip") {
        if (argc <= 3)
            std::cout << "Usage: zip <archive_name> <file1> <file2> ...";
        else {
            std::string archive_name(argv[2]);
            archive_name += ".xyz";
            for (int i = 3; i < argc; i++) {
                if (!check_file(argv[i])) {
                    std::cout << argv[i] << " does not exist or can't be opened";
                    return 0;
                }
            }
            std::vector<std::string> files;
            for (int i = 3; i < argc; i++)
                files.emplace_back(argv[i]);
            std::cout << "Zipping started\n";
            archiver::zip(files, archive_name);
            std::cout << "Zipping done!";
        }
    }
    else if (std::string(argv[1]) == "unzip") {
        if (argc == 2)
            std::cout << "Usage: unzip <archive_name>";
        else {
            std::string archive_name(argv[2]);
            if (!check_file(argv[2]))
                std::cout << argv[2] << " does not exist or can't be opened";
            std::cout << "Unzipping started\n";
            archiver::unzip(archive_name);
            std::cout << "Unzipping done!";
        }
    }
    else {
        std::cout << "Incorrect usage, type -help.";
    }
    return 0;
}