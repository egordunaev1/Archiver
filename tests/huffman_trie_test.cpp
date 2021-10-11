#include "../src/Archiver/huffman_trie.h"

#include "../src/archiver/huffman_trie.h"
#include <gtest/gtest.h>
#include <random>

TEST(huffman_trie, huffman_trie_test) {
    std::string _abc = "abcdefghijklmnopqrstuvwxyz";
    std::mt19937 rnd(123);
    for (int test = 0; test < 100; test++) {
        std::string abc = _abc.substr(1, rnd() % _abc.size());
        if (abc.empty())
            continue;
        std::unordered_map<char16_t, ull> freq;
        for (auto &i : abc)
            freq[i] = rnd();
        huffman_trie ht(freq);

        bytecode last;

        std::vector<std::pair<char, ull>> freq_sorted;
        for (auto &i : freq)
            freq_sorted.emplace_back(i);

        for (auto &i : freq) {
            ASSERT_GE(ht.get(i.second).size(),last.size());
        }
    }
}