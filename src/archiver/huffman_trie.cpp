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
huffman_trie::node::node(std::pair<ull, char>& value) {
    this->frequency = value.first;
    this->val = value.second;
    this->is_leaf = true;
}

// Конструктор вершины
huffman_trie::node::node(ull freq, std::shared_ptr <node> lc, std::shared_ptr <node> rc) {
    this->frequency = freq;
    this->left_child = std::move(lc);
    this->right_child = std::move(rc);
    this->val = 0;
}

// Построение дерева (возвращает указатель на корень)
nodeptr huffman_trie::build_tree(std::vector <std::pair<ull, char> >& frequency) {
    PriorityQueue<nodeptr> priorityQueue;
    for (auto& symbol : frequency)
        priorityQueue.push(std::make_shared<huffman_trie::node>(symbol));

    while (priorityQueue.size() > 1)
        priorityQueue.push(priorityQueue.pop() + priorityQueue.pop());
    return priorityQueue.size() ? priorityQueue.pop() : nullptr;
}

// Заполняет вектор парами символ - длина его кода
void huffman_trie::get_lens(const nodeptr &cur, std::vector<std::pair<short, char>>& out, short cur_len) {
    if (cur->is_leaf)
        return out.emplace_back(std::make_pair(cur_len, cur->val));
    cur_len++;
    if (cur->left_child)
        get_lens(cur->left_child, out, cur_len);
    if (cur->right_child)
        get_lens(cur->right_child, out, cur_len);
}

// Инкремент байткода
huffman_trie::bytecode& operator++(huffman_trie::bytecode& bc){
    short i = 0;
    for (; bc.bset[i]; ++i)
        bc.bset[i] = false;
    bc.bset[i] = true;
    return bc;
}

// Считает таблицу шифрования на основе длин кодов
void huffman_trie::make_canonical(std::vector<std::pair<short, char>>& lens) {
    std::sort(lens.begin(), lens.end());

    bytecode bc;
    bc.len = lens[0].first;
    this->table_[lens[0].second] = bc;

    for (int i = 1; i < lens.size(); i++) {
        ++bc;
        auto& cur = lens[i];
        bc.bset <<= cur.first - bc.len;
        bc.len = cur.first;
        this->table_[cur.second] = bc;
    }
}

// Единственный конструктор
huffman_trie::huffman_trie(std::vector <std::pair<ull, char> >& frequency) {
    // Исключение: нет символов
    if (frequency.empty())
        throw std::invalid_argument("alphabet size is null");

    // Исключение: текст не влезает в ull
    ull sum = 0;
    for (auto &i : frequency) {
        if (sum + i.first < sum)
            throw std::invalid_argument("text length is greater than unsigned long long can fit");
    }

    nodeptr root = this->build_tree(frequency);
    std::vector<std::pair<short, char>> lens;
    this->get_lens(root, lens);
    this->make_canonical(lens);
};

// Получение кода по символу
huffman_trie::bytecode huffman_trie::get(char chr) {
    return table_[chr];
}