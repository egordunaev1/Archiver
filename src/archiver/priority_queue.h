#pragma once

#include <vector>
#include <stdlib.h>

template<typename T>
class PriorityQueue {
private:
    std::vector<T>heap_;

    size_t parent(size_t el);
    size_t left_child(size_t el);
    size_t right_child(size_t el);

    void fix_up(size_t el);
    void fix_down(size_t el);
public:
    void push(T val);
    T pop();

    size_t size();

    PriorityQueue();
};

#include "priority_queue.cpp"