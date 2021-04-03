#pragma once

#include "list.h"

template <typename T, typename functor>
class hash_table
{
private:
    List<T>* table_ = nullptr;
    int length_ = 0;
    functor hash_;

public:
    hash_table(int size, functor hash_functor) : 
        length_(size),
        hash_(hash_functor)
    {
        table_ = new List<T>[size]();
    }

    ~hash_table()
    {
        delete[] table_;
    }

    hash_table<T, functor>& operator=(const hash_table<T, functor>& source) noexcept
    {
        hash_ = source.hash_;
        length_ = source.length_;
        table_ = source.table_;
    }

    hash_table(const hash_table<T, functor>& source)
    {
        hash_ = source.hash_;
        length_ = source.length_;
        table_ = new List<T>[length_]();

        for (int i = 0; i < length_; ++i)
            table_[i] = source.table_[i];
    }

    hash_table(hash_table<T, functor>&& source) noexcept
    {
        swap(*this, source);
    }

    hash_table& operator=(hash_table<T, functor>&& source) noexcept
    {
        swap(*this, source);

        return *this;
    }

    void add(T value)
    {
        int hashed = hash_(value) % TABLE_SIZE;
        table_[hashed].push_back(value);
    }

    void remove(T value)
    {
        int hashed = hash_(value) % TABLE_SIZE;
        table_[hashed].erase(value);
    }

    bool contains(T value) noexcept
    {
        int hashed = hash_(value) % TABLE_SIZE;

        return table_[hashed].contains(value);
    }

    void dump_lists_lens(std::ofstream& out) const
    {
        for (int i = 0; i < length_; ++i)
            out << table_[i].size() << "\n";
    }

    void clear()
    {
        for (int i = 0; i < TABLE_SIZE; ++i)
            table_[i].clear();
    }

    friend void swap(hash_table<T, functor>& first, hash_table<T, functor>& second) noexcept
    {
        std::swap(first.length, second.length);
        std::swap(first.hash, second.hash);
        std::swap(first.table, second.table);
    }
};
