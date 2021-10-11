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
        char16_t val;
        bool is_leaf = false;
        std::shared_ptr<node> left_child = nullptr;
        std::shared_ptr<node> right_child = nullptr;

        explicit node(std::pair<const char16_t, ull>&);
        node(ull freq, std::shared_ptr<node>lc, std::shared_ptr<node>rc);
        node() = default;
    };

    bytecode get(char16_t chr);
    const std::vector<char16_t>& get_order();

    explicit huffman_trie(std::unordered_map<char16_t, ull>& frequency);
private:
    std::shared_ptr<node> build_tree(std::unordered_map<char16_t, ull>& frequency);
    void get_lens(const std::shared_ptr<node>& cur, std::vector<std::pair<short, char16_t>>& out, short cur_len = 0);
    void make_canonical(std::vector<std::pair<short, char16_t>>& lens);

    std::vector<char16_t>order_;
    std::unordered_map<char16_t, bytecode>table_;
};

typedef std::shared_ptr<huffman_trie::node> nodeptr;
