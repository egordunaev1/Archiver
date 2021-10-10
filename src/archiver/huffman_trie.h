# pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>

#include "priority_queue.h"

class huffman_trie{
public:
    struct bytecode {
        std::bitset<256>bset;
        int len = 0;
    };

    struct node {
        int frequency;
        char val;
        bool is_leaf = false;
        std::shared_ptr<node> left_child = nullptr;
        std::shared_ptr<node> right_child = nullptr;

        node(std::pair<int, char>&);
        node(int freq, std::shared_ptr<node>lc, std::shared_ptr<node>rc);
        node() {};
    };

    bytecode get(char chr);

    huffman_trie(std::vector<std::pair<int, char> >& frequency);
private:
    std::shared_ptr<huffman_trie::node> build_tree(std::vector<std::pair<int, char> >& frequency);
    void make_canonical(std::shared_ptr<huffman_trie::node> root);

    std::vector<bytecode>table_;
};

typedef std::shared_ptr<huffman_trie::node> nodeptr;
