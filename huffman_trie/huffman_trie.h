# pragma once

#include <vector>
#include "../priority_queue.h"

class huffman_trie{
private:

public:
    struct node {
        node* left_child;
        node* right_child;
        std::vector<char>val;
    };
};

