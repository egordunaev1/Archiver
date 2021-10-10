#include "huffman_trie.h"

bool operator<(const nodeptr& _left, const nodeptr& _right) {
    return _left->frequency < _right->frequency ||
            _left->frequency == _right->frequency && _left->val > _right->val;
}

nodeptr operator+(const nodeptr& _left, const nodeptr& _right) {
    return std::make_shared<huffman_trie::node>(
        _left->frequency + _right->frequency,
        _left,
        _right
    );
}

huffman_trie::node::node(std::pair<int, char>& value) {
    this->frequency = value.first;
    this->val = { value.second };
    this->is_leaf = true;
}

huffman_trie::node::node(int freq, std::shared_ptr <node> lc, std::shared_ptr <node> rc) {
    this->frequency = freq;
    this->left_child = std::move(lc);
    this->right_child = std::move(rc);
    this->val = 0;
}

nodeptr huffman_trie::build_tree(std::vector <std::pair<int, char> >& frequency) {
    PriorityQueue<nodeptr> priorityQueue;
    for (auto& symbol : frequency)
        priorityQueue.push(std::make_shared<huffman_trie::node>(symbol));

    while (priorityQueue.size() > 1)
        priorityQueue.push(priorityQueue.pop() + priorityQueue.pop());
    return priorityQueue.size() ? priorityQueue.pop() : nullptr;
}

void _bytecode_increment(huffman_trie::bytecode& bc) {
    int i = 0;
    for (0; bc.bset[i]; ++i)
        bc.bset[i] = false;
    bc.bset[i] = true;
    bc.len += i == bc.len;
}

void huffman_trie::make_canonical(std::shared_ptr <huffman_trie::node> root) {
    auto alphabet_comparator = [](nodeptr& a, nodeptr& b){
        return !b->is_leaf || a->is_leaf && a->val < b->val;
    };

    std::vector<nodeptr>vec({std::move(root)});
    bytecode bc;
    bool first_use = true;
    int depth = 0;
    bc.len = 0;
    while (vec.size()) {
        std::sort(vec.begin(), vec.end(), alphabet_comparator);
        if (vec[0]->is_leaf) {
            if (first_use)
                first_use = false;
            else
                _bytecode_increment(bc);

            bc.bset <<= depth - bc.len;
            bc.len = depth;
            this->table_[static_cast<int>(vec[0]->val)] = bc;

            for (size_t i = 1; i < vec.size() && vec[i]->is_leaf; i++) {
                _bytecode_increment(bc);
                this->table_[static_cast<int>(vec[i]->val)] = bc;
            }
        }
        depth += 1;

        std::vector<nodeptr>tmp;
        for (auto &el : vec) {
            if (el->left_child)
                tmp.push_back(el->left_child);
            if (el->right_child)
                tmp.push_back(el->right_child);
        }
        vec = tmp;
    }
}

huffman_trie::huffman_trie(std::vector <std::pair<int, char> >& frequency) {
    this->table_.resize(frequency.size());
    nodeptr root = this->build_tree(frequency);
    this->make_canonical(root);
};

huffman_trie::bytecode huffman_trie::get(char chr) {
    return table_[chr];
}