#include "archiver.h"

// Запись кода символа в поток
void archiver::write(huffman_trie::bytecode bc, std::ofstream& out) {
    for (int i = bc.len - 1; i >= 0; --i) {
        buffer_.bset <<= 1;
        buffer_.bset[0] = bc.bset[i];
        buffer_.len++;
        if (buffer_.len == 8) {
            out << buffer_.bset.to_ulong();
            buffer_.bset.reset();
            buffer_.len = 0;
        }
    }
}

// Запись 9 символов в поток
void archiver::write(std::bitset<9>bs, std::ofstream& out) {
    for (int i = 8; i >= 0; --i) {
        buffer_.bset <<= 1;
        buffer_.bset[0] = bs[i];
        buffer_.len++;
        if (buffer_.len == 8) {
            out << static_cast<char>(buffer_.bset.to_ulong());
            buffer_.bset.reset();
            buffer_.len = 0;
        }
    }
}

void archiver::write_buffer(std::ofstream &out) {
    this->buffer_.bset <<= (9 - buffer_.len);
    out << static_cast<char>(this->buffer_.bset.to_ulong());
    buffer_.bset.reset();
    buffer_.len = 0;
}

void archiver::zip(std::vector <std::string> file_paths, std::string archive_name) {
    std::ofstream out(archive_name);
    for (int i = 0; i < file_paths.size(); i++) {
        this->zip_file(file_paths[i], out);
        if (i + 1 != file_paths.size())
            this->write(std::bitset<9>(this->ONE_MORE_FILE), out);
    }
    this->write(std::bitset<9>(this->ARCHIVE_END), out);
    this->write_buffer(out);
}

void archiver::zip_file(std::string &file_name, std::ofstream &out) {
    std::ifstream in(file_name);

    // Считаем встречаемость
    std::unordered_map<char, ull>freq;
    for (char byte; in >> std::noskipws >> byte;)
        freq[byte]++;

    // Пишем количество символов и символы
    huffman_trie huffmanTrie(freq);
    const std::vector<char>& symbols = huffmanTrie.get_order();
    this->write(std::bitset<9>(symbols.size()), out);
    for (auto&s : symbols)
        this->write(s, out);

    // Пишем количество символов каждой длины
    int len = 1;
    int amount = 0;
    for (auto&s : symbols) {
        ull cur_len = huffmanTrie.get(s).len;
        if (cur_len != len) {
            for (;len < cur_len; len++) {
                this->write(std::bitset<9>(amount), out);
                amount = 0;
            }
        }
        amount++;
    }

    // Пишем название файла
    for (auto& i : file_name)
        this->write(huffmanTrie.get(i), out);
    this->write(std::bitset<9>(this->FILENAME_END), out);

    // Пишем содержимое файла
    in.open(file_name);
    for (char byte; in >> std::noskipws >> byte;)
        this->write(huffmanTrie.get(byte), out);
}

