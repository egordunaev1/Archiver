#include "archiver.h"

// Запись кода символа в поток
void archiver::write_(huffman_trie::bytecode bc, std::ofstream& out) {
    for (int i = bc.len - 1; i >= 0; --i) {
        buffer_.bset <<= 1;
        buffer_.bset[0] = bc.bset[i];
        buffer_.len++;
        if (buffer_.len == 8) {
            out << static_cast<char>(buffer_.bset.to_ulong());
            buffer_.bset.reset();
            buffer_.len = 0;
        }
    }
}

// Запись 9 символов в поток
void archiver::write_(std::bitset<9>bs, std::ofstream& out) {
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

// Дописать буфер в поток
void archiver::write_buffer_(std::ofstream &out) {
    this->buffer_.bset <<= (9 - buffer_.len);
    out << static_cast<char>(this->buffer_.bset.to_ulong());
    buffer_.bset.reset();
    buffer_.len = 0;
}

void archiver::fill_buffer(std::ifstream &in) {
    buffer_.bset.reset();
    char byte;
    in >> std::noskipws >> byte;
    buffer_.bset = byte;
    buffer_.len = 8;
}

std::bitset<9> archiver::read_nine_(std::ifstream& in) {
    std::bitset<9>out;
    for (int i = 8; i >= 0; --i)
        out[i] = read_one_(in);
    return out;
}


bool archiver::read_one_(std::ifstream& in) {
    if (!buffer_.len)
        this->fill_buffer(in);
    bool res = buffer_.bset[7];
    buffer_.bset <<= 1;
    buffer_.len--;
}

// Сжать файлы
void archiver::zip(std::vector <std::string> file_paths, std::string archive_name) {
    std::ofstream out(archive_name);
    for (int i = 0; i < file_paths.size(); i++) {
        this->zip_file_(file_paths[i], out);
        if (i + 1 != file_paths.size())
            this->write_(std::bitset<9>(this->ONE_MORE_FILE), out);
    }
    this->write_(std::bitset<9>(this->ARCHIVE_END), out);
    this->write_buffer_(out);
}

// Записать в архив файл
void archiver::zip_file_(std::string &file_name, std::ofstream &out) {
    std::ifstream in(file_name);

    // Считаем встречаемость
    std::unordered_map<char16_t, ull>freq;
    for (char byte; in >> std::noskipws >> byte;)
        freq[byte]++;

    // Пишем количество символов и символы
    huffman_trie huffmanTrie(freq);
    const std::vector<char16_t>& symbols = huffmanTrie.get_order();
    this->write_(std::bitset<9>(symbols.size()), out);
    for (auto&s : symbols)
        this->write_(s, out);

    // Пишем количество символов каждой длины
    int len = 1;
    int amount = 0;
    for (auto&s : symbols) {
        ull cur_len = huffmanTrie.get(s).len;
        if (cur_len != len) {
            for (;len < cur_len; len++) {
                this->write_(std::bitset<9>(amount), out);
                amount = 0;
            }
        }
        amount++;
    }

    // Пишем название файла
    for (auto& i : file_name)
        this->write_(huffmanTrie.get(i), out);
    this->write_(std::bitset<9>(this->FILENAME_END), out);

    // Пишем содержимое файла
    in.open(file_name);
    for (char byte; in >> std::noskipws >> byte;)
        this->write_(huffmanTrie.get(byte), out);
}

void archiver::unzip(std::string archive) {
    std::ifstream in(archive);
    while (unzip_file_(in));
}

bool archiver::unzip_file_(std::ifstream &in) {
    int abc_length = this->read_nine_(in).to_ulong();
    std::string abc;
    for (int i = 0; i < abc_length; i++) {
        abc.push_back(char(this->read_nine_(in).to_ulong()));
    }
}

