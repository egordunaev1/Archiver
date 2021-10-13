#pragma once

#include <iostream>
#include <vector>

class reader {
private:
    std::istream& stream_;
    std::vector<bool>buffer_;

    bool read_from_buffer_(bool& out);
public:
    explicit reader(std::istream& stream) : stream_(stream) {};

    bool read(size_t amount, std::vector<bool>& out);
    bool read(size_t amount, unsigned long long &out);
    bool read(bool &out);
};