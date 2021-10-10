# pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>
#include <unordered_map>

#include "priority_queue.h"

using ull=unsigned long long;

class huffman_trie{
public:
    struct bytecode {
        std::bitset<256>bset;
        short len = 0;
    };

    struct node {
        ull frequency;
        char val;
        bool is_leaf = false;
        std::shared_ptr<node> left_child = nullptr;
        std::shared_ptr<node> right_child = nullptr;

        node(std::pair<ull, char>&);
        node(ull freq, std::shared_ptr<node>lc, std::shared_ptr<node>rc);
        node() {};
    };

    bytecode get(char chr);

    huffman_trie(std::vector<std::pair<ull, char> >& frequency);
private:
    std::shared_ptr<node> build_tree(std::vector<std::pair<ull, char> >& frequency);
    void get_lens(const std::shared_ptr<node>& cur, std::vector<std::pair<short, char>>& out, short cur_len = 0);
    void make_canonical(std::vector<std::pair<short, char>>& lens);

    std::unordered_map<char, bytecode>table_;
};

typedef std::shared_ptr<huffman_trie::node> nodeptr;
