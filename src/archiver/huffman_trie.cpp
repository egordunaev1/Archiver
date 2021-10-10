#include "huffman_trie.h"

// Перегрузка операторов сравнения указателей на вершины
bool operator<(const nodeptr& _left, const nodeptr& _right) {
    return _left->frequency < _right->frequency;
}

// Перегрузка операторов сложения указателей на вершины
nodeptr operator+(const nodeptr& _left, const nodeptr& _right) {
    return std::make_shared<huffman_trie::node>(
        _left->frequency + _right->frequency,
        _left,
        _right
    );
}

// Конструктор листа
huffman_trie::node::node(std::pair<int, char>& value) {
    this->frequency = value.first;
    this->val = { value.second };
    this->is_leaf = true;
}

// Конструктор вершины
huffman_trie::node::node(int freq, std::shared_ptr <node> lc, std::shared_ptr <node> rc) {
    this->frequency = freq;
    this->left_child = std::move(lc);
    this->right_child = std::move(rc);
    this->val = 0;
}

// Построение дерева (возвращает указатель на корень)
nodeptr huffman_trie::build_tree(std::vector <std::pair<int, char> >& frequency) {
    PriorityQueue<nodeptr> priorityQueue;
    for (auto& symbol : frequency)
        priorityQueue.push(std::make_shared<huffman_trie::node>(symbol));

    while (priorityQueue.size() > 1)
        priorityQueue.push(priorityQueue.pop() + priorityQueue.pop());
    return priorityQueue.size() ? priorityQueue.pop() : nullptr;
}

// Заполняет вектор парами символ - длина его кода
void huffman_trie::get_lens(nodeptr cur, std::vector<std::pair<int, int>>& out, int cur_len) {
    if (cur->is_leaf)
        return out.emplace_back(std::make_pair(cur_len, static_cast<int>(cur->val)));
    if (cur->left_child)
        get_lens(cur->left_child, out, cur_len + 1);
    if (cur->right_child)
        get_lens(cur->right_child, out, cur_len + 1);
}

// Инкремент байткода
huffman_trie::bytecode& operator++(huffman_trie::bytecode& bc){
    int i = 0;
    for (0; bc.bset[i]; ++i)
        bc.bset[i] = false;
    bc.bset[i] = true;
    bc.len += i == bc.len;
    return bc;
}

// Считает таблицу шифрования на основе длин кодов
void huffman_trie::make_canonical(std::vector<std::pair<int, int>>& out) {
    std::sort(out.begin(), out.end());

    bytecode bc;
    bc.len = out[0].first;
    this->table_[out[0].second] = bc;

    for (int i = 1; i < out.size(); i++) {
        ++bc;
        auto& cur = out[i];
        bc.bset <<= (std::max(0, cur.first - bc.len));
        bc.len = cur.first;
        this->table_[cur.second] = bc;
    }
}

// Единственный конструктор
huffman_trie::huffman_trie(std::vector <std::pair<int, char> >& frequency) {
    if (!frequency.size())
        throw std::invalid_argument("alphabet size is null");
    this->table_.resize(frequency.size());
    nodeptr root = this->build_tree(frequency);

    std::vector<std::pair<int, int>> lens;
    this->get_lens(root, lens);
    this->make_canonical(lens);
};

// Получение кода по символу
huffman_trie::bytecode huffman_trie::get(char chr) {
    return table_[chr];
}