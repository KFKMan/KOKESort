#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "KOKESort.h"

namespace py = pybind11;

// Python comparer fonksiyonunu saklamak için thread_local bir pointer
static thread_local py::function* current_py_comparer_ptr = nullptr;

// Sabit, yakalamayan karşılaştırma fonksiyonu: dış değişken (py_comparer) kullanılmaz,
// bunun yerine current_py_comparer_ptr üzerinden Python fonksiyonu çağrılır.
int comparer_bridge(const void* a, const void* b) {
    auto obj_a = *static_cast<py::object*>(const_cast<void*>(a));
    auto obj_b = *static_cast<py::object*>(const_cast<void*>(b));
    return (*current_py_comparer_ptr)(obj_a, obj_b).cast<int>();
}

void test_fn() 
{
    
}

PYBIND11_MODULE(KOKESortWrapper, m) 
{
    m.def("Test", &test_fn, "A simple example function");
    m.def("KOKESortV1", [](py::list arr, py::function py_comparer) {
        size_t size = arr.size();
        size_t elementSize = sizeof(py::object*);

        // Creating the c_arr array
        void* c_arr = malloc(size * sizeof(py::object*));
    
        // Storing the Pybind11 objects as py::object* in heap memory
        // This way, we allocate memory for each element
        for (size_t i = 0; i < size; ++i) {
            *((py::object**)c_arr + i) = new py::object(arr[i]);
        }
    
        // Setting the global pointer to the Python comparer function
        current_py_comparer_ptr = &py_comparer;
    
        // Calling the SortV1 function with the non-capturing comparer_bridge function
        void* result = SortV1(c_arr, size, elementSize, comparer_bridge);
    
        // Resetting the pointer once the operation is complete
        current_py_comparer_ptr = nullptr;
    
        // Converting the result to py::list and cleaning up each py::object in the heap
        py::list result_list;
        for (size_t i = 0; i < size; ++i) {
            auto obj = *((py::object**)result + i);
            result_list.append(obj);
            delete static_cast<py::object*>(*((py::object**)result + i)); // Free the heap memory for the py::object
        }
    
        // Free the c_arr memory
        free(c_arr);
    
        return result_list;
    }, "SortV1 sorts the array with a custom comparer");
}