#include "writer.h"

// Запись в буффер
void writer::write_in_buffer_(bool val) {
    this->buffer_.push_back(val);

    if (this->buffer_.size() == 8) {
        int byte = 0;

        for (int i = 0; i < 8; ++i)
            byte = (byte  << 1) + this->buffer_[i];
        this->stream_ << static_cast<char>(byte);
        this->buffer_.clear();
    }
}

// Запись массива бит
void writer::write(const std::vector<bool>& out) {
    for (bool val : out)
        this->write_in_buffer_(val);
}

// Запись числа
void writer::write(unsigned long long out, int len) {
    std::vector<bool>num;
    while (out || len) {
        len--;
        num.push_back(out % 2);
        out >>= 1;
    }
    std::reverse(num.begin(), num.end());
    this->write(num);
}

// Запись бита
void writer::write(bool out) {
    write_in_buffer_(out);
}

// Допись буффера при удалении класса
writer::~writer() {
    while (!buffer_.empty())
        write_in_buffer_(false);
}

