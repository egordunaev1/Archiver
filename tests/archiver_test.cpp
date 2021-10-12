#include "../src/archiver/archiver.h"
#include <gtest/gtest.h>
#include <random>

TEST(huffman_trie, huffman_trie_test) {
    std::ofstream first("test1.txt");
    std::ofstream second("test2.txt");

    std::string rand_text1 = "Abobik durak";
    std::string rand_text2 = "Bobik debil";

    first << rand_text1;
    second << rand_text2;

    std::vector<std::string>paths = {"test1.txt", "text2.txt"};

    archiver arch;
    arch.zip(paths, "myarch.xuy");
    std::remove("test1.txt");
    std::remove("test2.txt");
    arch.unzip("myarch.xuy");

    std::ifstream in("test1.txt", std::ifstream::binary);
    char c;
    while (in.get(c))
        std::cout << c;
}