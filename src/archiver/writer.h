#pragma once

#include <iostream>
#include <vector>


class writer {
private:
    std::ostream& stream_;
    std::vector<bool>buffer_;

    void write_in_buffer_(bool val);
public:
    explicit writer(std::ostream& stream) : stream_(stream) {};
    ~writer();

    void write(std::vector<bool> out);
    void write(unsigned long long out, int len);
    void write(bool out);
};
