#pragma once

#include "config.h"
#include "fast_string_functions.h"

template <typename T>
class Node
{
public:
    Node<T>* prev = nullptr;
    T value;

    Node(T value) : 
        value(value),
        prev(nullptr) 
    {}

    ~Node() = default;
};

template <typename T>
class List
{
private:
    int size_ = 0;
    Node<T>* back_ = nullptr;

public:
    List() = default;

    ~List()
    {
        clear();
    }

    void push_back(T value)
    {
        if (!size_)
        {
            back_ = new Node<T>(value);
            ++size_;

            return;
        }

        Node<T>* new_node = new Node<T>(value);
        new_node->prev = back_;
        back_ = new_node;

        ++size_;
    }

    void pop_back()
    {
        if (size_)
        {
            Node<T>* old_back = back_;
            back_ = back_->prev;
            --size_;

            delete old_back;
        }
    }

    void erase(T value)
    {
        Node<T>* current = back_;
        Node<T>* next = nullptr;

        while (current && current->value != value)
        {
            next = current;
            current = current->prev;
        }

        if (!current)
            return;

        next->prev = current->prev;
        delete current;

        --size_;
    }

    int size() const noexcept
    {
        return size_;
    }

    bool contains(T value) const noexcept
    {
        Node<T>* cur = back_;

        while (cur && fast_strcmp(cur->value, value) != 0)
            cur = cur->prev;

        /*while (cur && strcmp(cur->value, value) != 0)
            cur = cur->prev;*/

        return cur ? true : false;
    }

    void clear()
    {
        while (size_)
        {
            pop_back();
        }
    }

};