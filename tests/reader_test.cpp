#include <gtest/gtest.h>
#include <fstream>

#include "../src/archiver/writer.h"
#include "../src/archiver/reader.h"
#include <random>

const char* TEST_FILE = "TEST_FILE";

std::mt19937 mt(11);

void make_test_file() {

    std::ofstream out(TEST_FILE);
    char c = mt() % 256;
    for (int i = 0; i < 10; ++i)
        out.put(static_cast<char>(c));
    out.close();
}

TEST(READER_TEST, ONE_BIT_INPUT) {
    for (int TEST = 0; TEST < 1000; TEST++) {
        make_test_file();

        std::vector<bool> resCheck;
        std::vector<bool> resTest;

        std::ifstream inCheck(TEST_FILE, std::ifstream::binary);

        char c;
        while (inCheck.get(c)) {
            std::vector<bool> tmp;
            unsigned char _c = c;
            while (_c || tmp.size() != 8) {
                tmp.push_back(_c % 2);
                _c >>= 1;
            }
            for (int i = tmp.size() - 1; i >= 0; i--)
                resCheck.push_back(tmp[i]);
        }
        inCheck.close();

        std::ifstream inTest(TEST_FILE, std::ifstream::binary);
        reader READER(inTest);

        bool tmp;
        while (READER.read(tmp))
            resTest.push_back(tmp);
        inTest.close();


        ASSERT_EQ(resCheck, resTest);

        std::remove(TEST_FILE);
    }
}

TEST(READER_TEST, NUMBER_INPUT) {
    for (int TEST = 0; TEST < 100; TEST++) {
        make_test_file();

        std::vector<char> resCheck;
        std::vector<char> resTest;

        std::ifstream inCheck(TEST_FILE, std::ifstream::binary);

        char c;
        while (inCheck.get(c))
            resCheck.emplace_back(c);
        inCheck.close();

        std::ifstream inTest(TEST_FILE, std::ifstream::binary);
        reader READER(inTest);
        unsigned long long tmp;
        while (READER.read(8, tmp))
            resTest.emplace_back(char(tmp));

        ASSERT_EQ(resCheck, resTest);
        inTest.close();

        std::remove(TEST_FILE);
    }
}