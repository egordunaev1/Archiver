#include "../src/Archiver/huffman_trie.h"

#include "../src/archiver/huffman_trie.h";
#include <gtest/gtest.h>
#include <random>

TEST(huffman_trie, huffman_trie_test) {
    std::string _abc = "abcdefghijklmnopqrstuvwxyz";
    std::mt19937 rnd(123);
    for (int test = 0; test < 100; test++) {
        std::string abc = _abc.substr(1, rnd() % _abc.size());
        if (abc.empty())
            continue;
        std::vector<std::pair<ull, char>> freq;
        for (auto &i : abc)
            freq.emplace_back(std::make_pair(rnd(), i));
        huffman_trie ht(freq);

        huffman_trie::bytecode last;
        std::sort(freq.rbegin(), freq.rend());
        for (auto &i : freq) {
            ASSERT_GE(ht.get(i.second).len,last.len);
        }
    }
}