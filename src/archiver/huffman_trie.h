# pragma once

#include <vector>
#include "priority_queue.h"

class huffman_trie{
public:
    struct node {
        node* left_child;
        node* right_child;
        std::vector<char> val;
    };

    void build(std::vector<std::pair<char, int>> frequency);

    huffman_trie(std::vector<std::pair<char, int>> frequency);
private:
    PriorityQueue<node> priorityQueue;
};

