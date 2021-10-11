#include "writer.h"

// Запись в буффер
void writer::write_in_buffer_(bool val) {
    this->buffer_.emplace_back(val);

    if (this->buffer_.size() == 8) {
        char16_t byte;

        for (int i = 7; i >= 0; --i)
            byte = static_cast<char16_t>((byte  << 1) + this->buffer_[i]);
        this->stream_ << byte;
        this->buffer_.clear();
    }
}

// Запись массива бит
void writer::write(std::vector<bool> &in) {
    for (bool val : in)
        this->write_in_buffer_(val);
}

// Запись числа
void writer::write(unsigned long long &in) {
    std::vector<bool>num;
    while (in) {
        num.push_back(in % 2);
        in >>= 2;
    }
    this->write(num);
}

// Запись бита
void writer::write(bool &in) {
    write_in_buffer_(in);
}

// Допись буффера при удалении класса
writer::~writer() {
    while (!buffer_.empty())
        write_in_buffer_(false);
}

