#include "../src/archiver/archiver.h"
#include <gtest/gtest.h>
#include <random>

TEST(archiver_test, archiver_hand_test) {
    std::ofstream first("test1.txt");
    std::ofstream second("test2.txt");

    std::string rand_text1 = "Abobik durak";
    std::string rand_text2 = "Bobik debil";

    first << rand_text1;
    second << rand_text2;
    first.close();
    second.close();

    std::vector<std::string>paths = {"folder/z.mp4", "folder/x.mp4"};

    archiver::zip(paths, "myarch.xuy");
    archiver::unzip("myarch.xuy");
}