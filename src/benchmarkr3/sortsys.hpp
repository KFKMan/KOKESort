#pragma once
#include <cpp-sort/sorters.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include <KOKESort.h>
#include <master.h>
#include <cwrappersys.hpp>

using SorterFunction = std::function<void(std::vector<int>&)>;

#define SORT_VEC std::vector<int>& v
#define SORT_FN(name, func) { name, [](SORT_VEC) { func(v); } }
#define SORT_CPP(name, sorter) SORT_FN(name, cppsort::sorter{})
#define SORT_C(name, cfunc) SORT_FN(name, wrap_c_sort<int>(cfunc))
#define SORT_C_KOKESortV2(name, cfunc) SORT_FN(name, wrap_c_sort_for_KOKESortV2<int>(cfunc))

inline std::unordered_map<std::string, SorterFunction> get_sorters() {
    return 
    {
        SORT_CPP("block",        block_sorter),
        SORT_C("c_bubble",       bubbleSort),
        SORT_C("c_heap", heapSort),
        SORT_C("c_insertion", insertionSort),
        SORT_C("c_merge", mergeSort),
        SORT_C("c_quick", quickSort),
        SORT_C("c_selection", selectionSort),
        SORT_C("c_kokev1", SortV1Self),
        SORT_C_KOKESortV2("c_kokev2_quick", quickSort),
        SORT_CPP("counting",     counting_sorter),
        SORT_CPP("drop_merge",   drop_merge_sorter),
        SORT_CPP("heap",         heap_sorter),
        SORT_CPP("insertion",    insertion_sorter),
        SORT_CPP("merge",        merge_sorter),
        SORT_CPP("pdq",          pdq_sorter),
        SORT_CPP("poplar",       poplar_sorter),
        SORT_CPP("quick",        quick_sorter),
        SORT_CPP("selection",    selection_sorter),
        SORT_CPP("ska",          ska_sorter),
        SORT_CPP("smooth",       smooth_sorter),
        SORT_CPP("splay",        splay_sorter),
        SORT_CPP("spin",         spin_sorter),
        SORT_CPP("spread",       spread_sorter),
        SORT_CPP("tim",          tim_sorter),
        SORT_CPP("verge",        verge_sorter),
    };
}

