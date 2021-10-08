#pragma once

#include <stdexcept>
#include <iostream>
#include "priority_queue.h"


// Родитель
template<typename T>
size_t PriorityQueue<T>::parent(size_t el) {
    return el >> 1;
}

// Левый сын
template<typename T>
size_t PriorityQueue<T>::left_child(size_t el) {
    return el << 1;
}

// Правый сын
template<typename T>
size_t PriorityQueue<T>::right_child(size_t el) {
    return (el << 1) | 1;
}

// Инициализация
template<typename T>
PriorityQueue<T>::PriorityQueue() {
    heap_.emplace_back();
}

// Размер кучи
template<typename T>
size_t PriorityQueue<T>::size() {
    return this->heap_.size() - 1;
}

// Чиним кучу вверх
template<typename T>
void PriorityQueue<T>::fix_up(size_t el) {
    while (el != 1 && this->heap_[el] < this->heap_[parent(el)]) {
        std::swap(this->heap_[el], this->heap_[parent(el)]);
        el = parent(el);
    }
}

// Чиним кучу вниз
template<typename T>
void PriorityQueue<T>::fix_down(size_t el) {
    while (left_child(el) < this->heap_.size()) {
        size_t child1 = this->left_child(el);
        size_t child2 = this->right_child(el);
        if (child2 < this->heap_.size() && this->heap_[child2] < this->heap_[child1])
            child1 = child2;
        if (this->heap_[child1] >= this->heap_[el])
            return;
        std::swap(this->heap_[el], this->heap_[child1]);
        el = child1;
    }
}

// Реализация функции push
template<typename T>
void PriorityQueue<T>::push(T val) {
    this->heap_.emplace_back(val);
    this->fix_up(this->heap_.size() - 1);
}

// Реализация функции pop
template<typename T>
T PriorityQueue<T>::pop() {
    if (!this->size())
        throw std::out_of_range("Trying to pop from empty PriorityQueue.");
    std::swap(this->heap_[this->heap_.size() - 1], this->heap_[1]);
    T top_element = this->heap_.back();
    this->heap_.pop_back();
    if (this->size())
        this->fix_down(1);
    return top_element;
}