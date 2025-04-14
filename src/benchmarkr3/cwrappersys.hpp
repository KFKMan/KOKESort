#pragma once
#include <functional>

using CompareFunction = int(*)(const void*, const void*);
using CSortFunction = void(*)(void* arr, size_t size, size_t elemSize, CompareFunction cmp);

template<typename T>
int default_compare(const void* a, const void* b) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    if (lhs < rhs) 
    {
        return -1;
    }
    if (rhs < lhs) 
    {
        return 1;
    }
    return 0;
}

template<typename T>
std::function<void(std::vector<T>&)> wrap_c_sort(CSortFunction sorter) {
    return [sorter](std::vector<T>& vec) 
    {
        if (vec.empty())
        {
            return;
        }
        sorter(static_cast<void*>(vec.data()), vec.size(), sizeof(T), default_compare<T>);
    };
}