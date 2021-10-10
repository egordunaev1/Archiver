#include "../src/Archiver/huffman_trie.h"

#include "../src/archiver/huffman_trie.h";
#include <gtest/gtest.h>

TEST(huffman_trie, huffman_trie_test) {
    std::string abc = "gfedcab";
    int freq[] = {1000, 1000, 1000, 1000, 1000, 1000, 1000};
    std::vector<std::pair<int, char>>vec(abc.size());
    for (int i = 0; i < abc.size(); i++)
        vec[i] = {freq[i], abc[i]};

    huffman_trie ht(vec);
    for (int i = 0; i < abc.size(); i++)
        std::cout << abc[i] << ' ' << freq[i] << ' ' << ht.get(abc[i]).bset.to_string().substr(256-ht.get(abc[i]).len, 256) <<' '<<ht.get(abc[i]).len << '\n';
}