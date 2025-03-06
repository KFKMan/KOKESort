#include <pybind11/pybind11.h>
#include "wrapper.hpp"

namespace py = pybind11;

PYBIND11_MODULE(KOKESortWrapper, m) 
{
    m.def("SortV1", [](py::list arr) {
        size_t size = arr.size();
        VariableType* c_arr = new VariableType[size];

        for (size_t i = 0; i < size; ++i) {
            c_arr[i] = arr[i].cast<VariableType>();
        }

        VariableType* result = SortV1(c_arr, size);

        py::list result_list;
        for (size_t i = 0; i < size; ++i) {
            result_list.append(result[i]);
        }

        delete[] c_arr;

        return result_list;
    }, "SortV1 sorts the array");
}