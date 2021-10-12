# pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <map>
#include "priority_queue.h"

using ull=unsigned long long;
using bytecode=std::vector<bool>;

class huffman_trie {
public:
    struct node {
        ull frequency;
        int val;
        bool is_leaf = false;
        std::shared_ptr<node> left_child = nullptr;
        std::shared_ptr<node> right_child = nullptr;

        explicit node(std::pair<const int, ull>&);
        node(ull freq, std::shared_ptr<node>lc, std::shared_ptr<node>rc);
        node() = default;
    };

    bytecode get(int chr);
    const std::vector<int>& get_order();
    static std::unordered_map<int, bytecode> make_canonical(std::vector<std::pair<int, int>>& lens);

    explicit huffman_trie(std::unordered_map<int, ull>& frequency);
private:
    std::shared_ptr<node> build_tree(std::unordered_map<int, ull>& frequency);
    void get_lens(const std::shared_ptr<node>& cur, std::vector<std::pair<int, int>>& out, short cur_len = 0);

    std::vector<int>order_;
    std::unordered_map<int, bytecode>table_;
};

typedef std::shared_ptr<huffman_trie::node> nodeptr;
