#pragma once
#include <functional>
#include <sort.h>
#include <KOKESortV2.h>
#include <config.hpp>

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
std::function<void(std::vector<T>&)> wrap_c_sort(SortFunction sorter) {
    return [sorter](std::vector<T>& vec) 
    {
        if (vec.empty())
        {
            std::cerr << "Error: Vector is empty!" << std::endl;
            return;
        }
        sorter(static_cast<void*>(vec.data()), vec.size(), sizeof(T), default_compare<T>);
    };
}

size_t indexer(const void *valptr, const void *data)
{
    const GenerateValueType val1 = *(const GenerateValueType *)valptr;
    return val1 / Divider;
}

template<typename T>
std::function<void(std::vector<T>&)> wrap_c_sort_for_KOKESortV2(SortFunction sorter) {
    return [sorter](std::vector<T>& vec) 
    {
        if (vec.empty())
        {
            std::cerr << "Error: Vector is empty!" << std::endl;
            return;
        }
        LastSortData = SortV2(static_cast<void*>(vec.data()), vec.size(), sizeof(T), SpaceCount, indexer, nullptr, default_compare<T>, sorter);
    };
}