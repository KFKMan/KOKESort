// #include <pybind11/pybind11.h>
#include <nanobind/nanobind.h>
// #include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include "KOKESort.h"
#include "Global.h"

// namespace py = pybind11;
namespace py = nanobind;

// py::function
thread_local py::callable current_py_comparer;

int comparer_bridge(const void *a, const void *b)
{
    //std::cout << "[comparer_bridge] Called" << std::endl;
    //py::object* obj_a_ptr = static_cast<py::object *>(const_cast<void *>(a));
    //py::object* obj_b_ptr = static_cast<py::object *>(const_cast<void *>(b));

    const py::object* obj_a_ptr = *(const py::object**)a;
    const py::object* obj_b_ptr = *(const py::object**)b;

    const py::object& obj_a = *obj_a_ptr;
    const py::object& obj_b = *obj_b_ptr;
    try
    {
        //std::cout << "[comparer_bridge] Try-Catch" << std::endl;
        if (!current_py_comparer)
        {
            std::cerr << "[comparer_bridge] Python comparer is not set properly!" << std::endl;
            return 0;
        }

        //std::cout << "[comparer_bridge] Calling Python Comparer" << std::endl;

        // py::object result_obj = current_py_comparer(obj_a, obj_b);
        py::object result_obj = current_py_comparer(obj_a, obj_b);

        //std::cout << "[comparer_bridge] Called Python Comparer" << std::endl;
        if (result_obj.is_none())
        {
            std::cerr << "[comparer_bridge] Python function returned None." << std::endl;
            return 0;
        }
        //std::cout << "[comparer_bridge] Casting result" << std::endl;
        // int result = result_obj.cast<int>();
        int result = 0;
        if (py::isinstance<py::int_>(result_obj))
        {
            result = py::cast<int>(result_obj);
        }
        else
        {
            std::cerr << "[comparer_bridge] Return type is not integer" << std::endl;
        }
        //std::cout << "[comparer_bridge] Compared: " << result << std::endl;
        return result;
    }
    /*
    catch (const py::error_already_set& e)
    {
        std::cerr << "[comparer_bridge] Exception Found - PY" << std::endl;
        std::cerr << "[comparer_bridge] Python error: " << e.what() << std::endl;
        return 0;
    }
    */
    catch (const std::exception &e)
    {
        std::cerr << "[comparer_bridge] Exception Found - STD" << std::endl;
        std::cerr << "[comparer_bridge] Exception: " << e.what() << std::endl;
        return 0;
    }
    catch (...)
    {
        std::cerr << "[comparer_bridge] Unknown exception occurred." << std::endl;
        return 0;
    }
    // return (*current_py_comparer_ptr)(obj_a, obj_b).cast<int>();
}

void test_fn()
{
    //std::cout << "[test_fn] Called test function." << std::endl;
}

void test_static_fn()
{
    int res = TestFunction();
    //std::cout << "[test_static_fn] Called static test function. With result" << res << std::endl;
}

py::list SortV1Wrap(py::list arr, py::callable py_comparer, bool log = false)
{
    try
    {
        //if (log)
            //std::cout << "[SortV1] Starting sort..." << std::endl;

        size_t size = arr.size();
        size_t elementSize = sizeof(py::object *);

        //if (log)
            //std::cout << "[SortV1] Array Size " << size << " Element Size (Pointer) " << elementSize << std::endl;
        //if (log)
            //std::cout << "[SortV1] Allocating..." << std::endl;
        // py::object** c_array = static_cast<py::object**>(malloc(size * elementSize));

        std::vector<py::object *> c_array;
        c_array.reserve(size);

        /*
        if (!c_array)
        {
            std::cerr << "[SortV1] Memory allocation failed." << std::endl;
            py::list empty_list;
            return empty_list;
        }
        */

        for (size_t i = 0; i < size; ++i)
        {
            py::object obj = arr[i];
            py::object *objPtr = new py::object(obj); //&obj; // new py::object(obj);
            c_array.push_back(objPtr);
        }

        current_py_comparer = py_comparer;

        //std::cout << "Value is " << py::cast<int>(py_comparer(arr[0], arr[1])) << std::endl;
        //std::cout << "Value from vector is " << py::cast<int>(py_comparer(c_array.at(0), c_array.at(1))) << std::endl;

        //std::cout << "Using thread local comparer" << std::endl;
        //std::cout << "Value is " << py::cast<int>(current_py_comparer(arr[0], arr[1])) << std::endl;
        //std::cout << "Value from vector is " << py::cast<int>(current_py_comparer(c_array.at(0), c_array.at(1))) << std::endl;

        //if (log)
            //std::cout << "[SortV1] Sorting..." << std::endl;

        SortV1Self(static_cast<void *>(c_array.data()), size, elementSize, comparer_bridge);

        //if (log)
            //std::cout << "[SortV1] Converting..." << std::endl;
        py::list result_list;
        for (size_t i = 0; i < size; ++i)
        {
            result_list.append(*c_array[i]);
            // delete c_array[i];
        }

        //if (log)
            //std::cout << "[SortV1] Sort finished." << std::endl;
        return result_list;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[SortV1] Exception occurred: " << e.what() << std::endl;
        py::list empty_list;
        return empty_list; // Hata durumunda boş liste döndürüyoruz
    }
    catch (...)
    {
        std::cerr << "[SortV1] Unknown error occurred." << std::endl;
        py::list empty_list;
        return empty_list; // Bilinmeyen hata durumunda boş liste döndürüyoruz
    }
}

// PYBIND11_MODULE(KOKESortWrapper, m)
NB_MODULE(KOKESortWrapper, m)
{
    m.def("Test", &test_fn, "A simple example function");
    m.def("TestStatic", &test_static_fn, "Static linking example function");

    // py::function py_comparer
    m.def("SortV1", &SortV1Wrap,
          py::arg("arr"), py::arg("comparer"), py::arg("log") = false);
}