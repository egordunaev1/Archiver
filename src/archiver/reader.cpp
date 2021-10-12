#include "reader.h"

// Наполнение буффера
bool reader::read_from_buffer_(bool &out) {
    if (this->buffer_.empty()) {
        char byte;
        if (!this->stream_.get(byte))
            return false;
        this->buffer_.clear();

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
    out.clear();
    for (; amount; amount--) {
        bool bit;
        if (!this->read_from_buffer_(bit))
            return false;
        out.push_back(bit);
    }
    return true;
}

// Чтение массива бит в виде числа
bool reader::read(size_t amount, unsigned long long &out) {
    out = 0;

    for (; amount; amount--) {
        bool bit;
        if (!this->read_from_buffer_(bit))
            return false;
        out += bit << (amount - 1);
    }
    return true;
}

// Чтение бита
bool reader::read(bool &out) {
    if (!this->read_from_buffer_(out))
        return false;
    return true;
}

