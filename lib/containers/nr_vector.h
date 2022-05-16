
// -*- coding:utf-8-unix -*-
/*!
    @file       nr_vector.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-04

    @brief      Non resizable vector
    @description
        
    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__NR_VECTOR_H
#define file__NR_VECTOR_H

#include <cstdint>
#include <memory>
#include <algorithm>

template <typename T>
class nr_vector
{
    nr_vector(nr_vector<T> const& other)
    : m_size(other.size())
    , m_data(new T[other.size()])
    {
        std::copy(other.begin(), other.end(), m_data.get());
    }

    nr_vector(size_t count, const T& value = {})
    : m_size(count)
    , m_data(new T[count](value))
    {
    }

    ~nr_vector()
    {

    }

    nr_vector& operator = (nr_vector& other)
    {
        nr_vector c(other);
        swap(c);
    }

    void swap(nr_vector<T>& other) 
    {
        std::swap(m_size, other.m_size);
        std::swap(m_data, other.m_data);
    }

    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }

    typedef T* iterator;
    typedef T const* const_iterator;

    iterator begin() { return data(); }
    iterator end() { return data() + m_size; }

    const_iterator begin() const { return data(); }
    const_iterator end() const { return data() + m_size; }

    T* data() { return m_data.get(); }
    T const* data() const { return m_data.get(); }

    T& operator[] (size_t idx) { return m_data.get()[idx]; }
    T const& operator[] (size_t idx) const { return m_data.get()[idx]; }

private:
    size_t m_size = 0;
    std::unique_ptr<T[]> m_data;
    //T* m_data;
};

#endif // file__NR_VECTOR_H
