#include <gtest/gtest.h>
#include <fstream>

#include "../src/archiver/writer.h"
#include "../src/archiver/reader.h"
#include <random>

const char* TEST_FILE = "TEST_FILE";

std::mt19937 mt(100);

void make_test_file() {

    std::ofstream out(TEST_FILE);
    for (int i = 0; i < 10; ++i)
        out.put(static_cast<char>(mt() % 256));
    out.close();
}

TEST(READER_TEST, ONE_BIT_INPUT) {
    for (int TEST = 0; TEST < 100; TEST++) {
        make_test_file();

        std::vector<bool> resCheck;
        std::vector<bool> resTest;

        std::ifstream inCheck(TEST_FILE);

        char c;
        while (inCheck.get(c)) {
            std::vector<bool> tmp;
            unsigned char _c = c;
            while (_c || tmp.size() != 8) {
                tmp.emplace_back(_c % 2);
                _c >>= 1;
            }
            for (int i = tmp.size() - 1; i >= 0; i--)
                resCheck.emplace_back(tmp[i]);
        }
        inCheck.close();

        std::ifstream inTest(TEST_FILE);
        reader READER(inTest);

        bool tmp;
        while (READER.read(tmp))
            resTest.emplace_back(tmp);
        inTest.close();


        ASSERT_EQ(resCheck, resTest);

        std::remove(TEST_FILE);
    }
}

TEST(READER_TEST, MANY_BIT_INPUT) {
    for (int TEST = 0; TEST < 10000; TEST++) {
        make_test_file();

        std::ifstream inCheck(TEST_FILE);

        std::vector<char>v;

        char c;
        while (inCheck.get(c)) {
            v.push_back(c);
        }
        std::cout << int(c) << ' ';
        inCheck.close();


        std::cout <<TEST << '\n';
        ASSERT_EQ(v.size(), 10);
    }
}

/*
 * std::vector<bool> tmp;
            unsigned char _c = c;
            while (_c || tmp.size() != 8) {
                tmp.emplace_back(_c % 2);
                _c >>= 1;
            }
            for (int i = tmp.size() - 1; i >= 0; i--)
                resCheck.emplace_back(tmp[i]);
            int x = 2 + c;
 *         std::ifstream inTest(TEST_FILE);
        reader READER(inTest);
        std::vector<bool> tmp;
        while (READER.read(5, tmp))
            for (auto i: tmp)
                resTest.emplace_back(i);
 *
 */

TEST(READER_TEST, NUMBER_INPUT) {
    for (int TEST = 0; TEST < 100; TEST++) {
        make_test_file();

        std::vector<char> resCheck;
        std::vector<char> resTest;

        std::ifstream inCheck(TEST_FILE);

        char c;
        while (inCheck.get(c))
            resCheck.emplace_back(c);
        inCheck.close();

        std::ifstream inTest(TEST_FILE);
        reader READER(inTest);
        unsigned long long tmp;
        while (READER.read(8, tmp))
            resTest.emplace_back(char(tmp));

        ASSERT_EQ(resCheck, resTest);
        inTest.close();

        std::remove(TEST_FILE);
    }
}