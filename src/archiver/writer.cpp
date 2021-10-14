#include "writer.h"

// Запись в буффер
void writer::write_in_buffer_(bool val) {
    this->buffer_.push_back(val);

    if (this->buffer_.size() == 8) {
        unsigned char byte = 0;

        for (int i = 0; i < 8; ++i)
            byte = (byte << 1) + this->buffer_[i];
        this->stream_.put(static_cast<char>(byte));
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
    while (len--) {
        this->write_in_buffer_(out & 1);
        out >>= 1;
    }
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

// Конструктор от файла
writer::writer(std::string file) {
    this->stream_.open(this->file_ = std::move(file), std::ofstream::binary);
}

// Путь к файлу
std::string writer::get_path() {
    return this->file_;
}

// Имя файла
std::string writer::get_name() {
    std::string res;
    for (size_t i = this->file_.size() - 1; i >= 0; i--) {
        if (this->file_[i] == '\\' || this->file_[i] == '/')
            break;
    }
    std::reverse(res.begin(), res.end());
    return res;
}
