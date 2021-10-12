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
huffman_trie::node::node(std::pair<const int, ull>& value) {
    this->frequency = value.second;
    this->val = value.first;
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
nodeptr huffman_trie::build_tree(std::unordered_map<int, ull>& frequency) {
    PriorityQueue<nodeptr> priorityQueue;
    for (auto& symbol : frequency)
        priorityQueue.push(std::make_shared<huffman_trie::node>(symbol));

    while (priorityQueue.size() > 1)
        priorityQueue.push(priorityQueue.pop() + priorityQueue.pop());
    return priorityQueue.size() ? priorityQueue.pop() : nullptr;
}

// Заполняет вектор парами символ - длина его кода
void huffman_trie::get_lens(const nodeptr &cur, std::vector<std::pair<int, int>>& out, short cur_len) {
    if (cur->is_leaf)
        return void(out.emplace_back(std::make_pair(cur_len, cur->val)));
    cur_len++;
    if (cur->left_child)
        get_lens(cur->left_child, out, cur_len);
    if (cur->right_child)
        get_lens(cur->right_child, out, cur_len);
}

// Инкремент байткода
void _bytecode_increment(bytecode& bc){
    short i = 0;
    for (; i < bc.size() && bc[i]; ++i)
        bc[i] = false;

    if (i == bc.size())
        bc.push_back(true);
    else
        bc[i] = true;
}

// Считает таблицу шифрования на основе длин кодов
std::unordered_map<int, bytecode> huffman_trie::make_canonical(std::vector<std::pair<int, int>>& lens) {
    std::unordered_map<int, bytecode>table;

    std::sort(lens.begin(), lens.end());

    bytecode bc(lens[0].first);
    table[lens[0].second] = bc;

    for (int i = 1; i < lens.size(); i++) {
        _bytecode_increment(bc);
        auto& cur = lens[i];
        bc.insert(bc.begin(), cur.first - bc.size(), false);
        table[cur.second] = bc;
        std::reverse(table[cur.second].begin(), table[cur.second].end());
    }
    return table;
}

// Единственный конструктор
huffman_trie::huffman_trie(std::unordered_map<int, ull>& frequency) {
    // Исключение: нет символов
    if (frequency.empty())
        throw std::invalid_argument("alphabet size is null");

    // Исключение: текст не влезает в ull
    ull sum = 0;
    for (auto &i : frequency) {
        if (sum + i.first < sum)
            throw std::invalid_argument("text length is greater than unsigned long long can fit");
        sum += i.first;
    }

    nodeptr root = this->build_tree(frequency);
    std::vector<std::pair<int, int> > lens;
    this->get_lens(root, lens);
    this->table_ = huffman_trie::make_canonical(lens);
    for (auto &i : table_)
        this->order_.emplace_back(i.second.size(), i.first);
    std::sort(this->order_.begin(), this->order_.end());
};

// Получение кода по символу
bytecode huffman_trie::get(int chr) {
    return table_[chr];
}

const std::vector<std::pair<int, int>>& huffman_trie::get_order() {
    return this->order_;
}
