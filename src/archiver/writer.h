#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>


class writer {
private:
    std::string file_;
    std::ofstream stream_;
    std::vector<bool>buffer_;

    void write_in_buffer_(bool val);
public:
    explicit writer(const std::string& file);
    ~writer();

    std::string get_path();
    std::string get_name();

    void write(const std::vector<bool>& out);
    void write(unsigned long long out, int len);
    void write(bool out);
};
