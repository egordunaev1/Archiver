# pragma once

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

#include "priority_queue.h"

using ull=unsigned long long;
using bitcode=std::vector<bool>;

class huffman_trie {
public:
    struct node {
        ull frequency{};
        int val{};
        std::shared_ptr<node> left_child = nullptr;
        std::shared_ptr<node> right_child = nullptr;

        bool is_leaf() const;

        node() = default;
        explicit node(const std::pair<const int, ull>& value) : frequency(value.second), val(value.first) {}
        node(const ull& freq,
             std::shared_ptr<node> lc,
             std::shared_ptr<node> rc
             ) : frequency(freq), left_child(std::move(lc)), right_child(std::move(rc)) {}
    };

    bitcode get(int chr);
    const std::vector<std::pair<int, int>>& get_order();
    static std::unordered_map<int, bitcode> make_canonical(std::vector<std::pair<int, int>>& lens);

    explicit huffman_trie(const std::unordered_map<int, ull>& frequency);
private:
    static std::shared_ptr<node> build_tree(const std::unordered_map<int, ull>& frequency);
    void get_lens(const std::shared_ptr<node>& cur, std::vector<std::pair<int, int>>& out, short cur_len = 0);

    std::unordered_map<int, bitcode>table_;
    std::vector<std::pair<int, int>>order_;
};
typedef std::shared_ptr<huffman_trie::node> nodeptr;