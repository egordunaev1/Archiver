#pragma once

#include <fstream>
#include <vector>
#include <algorithm>

class reader {
private:
    std::string file_;
    std::ifstream stream_;
    std::vector<bool>buffer_;

    bool read_from_buffer_(bool& out);
public:
    explicit reader(std::string file);

    void reopen();

    std::string get_path();
    std::string get_name();

    bool read(size_t amount, std::vector<bool>& out);
    bool read(size_t amount, unsigned long long &out);
    bool read(bool &out);
};