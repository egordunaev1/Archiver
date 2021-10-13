#include "reader.h"

// Наполнение буффера
bool reader::read_from_buffer_(bool &out) {
    if (this->buffer_.empty()) {
        char byte;
        if (!this->stream_.get(byte))
            return false;

        unsigned char _byte = byte;
        while (_byte || buffer_.size() != 8) {
            buffer_.push_back(static_cast<bool>(_byte % 2));
            _byte >>= 1;
        }
    }
    out = buffer_.back();
    buffer_.pop_back();

    return true;
}

// Чтение массива бит
bool reader::read(size_t amount, std::vector<bool> &out) {
    out.resize(amount);
    for (size_t i = 0; i < amount; i++) {
        bool bit;
        if (!this->read_from_buffer_(bit))
            return false;
        out[i] = bit;
    }
    return true;
}

// Чтение массива бит в виде числа
bool reader::read(size_t amount, unsigned long long &out) {
    out = 0;
    for (char shift = 0; amount; amount--, shift++) {
        bool bit;
        if (!this->read_from_buffer_(bit))
            return false;
        out |= bit << shift;
    }
    return true;
}

// Чтение бита
bool reader::read(bool &out) {
    if (!this->read_from_buffer_(out))
        return false;
    return true;
}

// Конструктор от файла
reader::reader(const std::string &file) {
    this->stream_.open(this->file_ = file, std::ifstream::binary);
}

// Повторное открытие файла
void reader::reopen() {
    this->stream_.close();
    this->stream_.open(this->file_, std::ifstream::binary);
}

// Путь к файлу
std::string reader::get_path() {
    return this->file_;
}

// Имя файла
std::string reader::get_name() {
    std::string res;
    for (size_t i = this->file_.size() - 1; i >= 0; i--) {
        if (this->file_[i] == '\\' || this->file_[i] == '/')
            break;
        res.push_back(this->file_[i]);
    }
    std::reverse(res.begin(), res.end());
    return res;
}

