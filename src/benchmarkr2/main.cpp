#include <KOKESort.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;
using namespace std::chrono;

int intComparer(const void* val1ptr, const void* val2ptr)
{
    const int val1 = *(const int*)val1ptr;
    const int val2 = *(const int*)val2ptr;
    if(val1 > val2)
    {
        return 1;
    }
    if(val1 == val2)
    {
        return 0;
    }
    return -1;
}

struct CArray {
    int* data;
    size_t size;
};

CArray parseCArray(const std::string& line) {
    std::istringstream ss(line);
    std::vector<int> temp;
    int num;
    while (ss >> num) {
        temp.push_back(num);
    }
    CArray carr;
    carr.size = temp.size();
    carr.data = new int[carr.size];
    std::copy(temp.begin(), temp.end(), carr.data);
    return carr;
}

std::vector<CArray> readCArraysFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return {};
    }

    std::vector<CArray> arrays;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            arrays.push_back(parseCArray(line));
        }
    }
    return arrays;
}

class DurationFormatter {
    public:
        static std::string format(std::chrono::nanoseconds ns) {
            using namespace std::chrono;
    
            if (ns < microseconds(1)) {
                return std::to_string(ns.count()) + " ns";
            } else if (ns < milliseconds(1)) {
                return std::to_string(duration_cast<microseconds>(ns).count()) + " µs";
            } else if (ns < seconds(1)) {
                return std::to_string(duration_cast<milliseconds>(ns).count()) + " ms";
            } else if (ns < minutes(1)) {
                return std::to_string(duration_cast<seconds>(ns).count()) + " s";
            } else {
                return std::to_string(duration_cast<minutes>(ns).count()) + " min";
            }
        }
    };

void benchmarkSortV1(const std::string& benchName, const int* data, size_t size) {
    int* copy = const_cast<int*>(data);

    auto start = std::chrono::high_resolution_clock::now();

    int res = SortV1Self(copy, size, intComparer, sizeof(int));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << benchName << " | SortV1 | " << size << " --> " << duration.count() << "ns" << " || " << DurationFormatter::format(duration) << std::endl;
}

void benchmarkQuickSort(const std::string& benchName, const int* data, size_t size) {
    int* copy = new int[size];
    std::copy(data, data + size, copy);

    if (copy == nullptr) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::qsort(copy, size, sizeof(int), intComparer);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << benchName << " | QuickSort | " << size << " --> " << duration.count() << "ns" << " || " << DurationFormatter::format(duration) << std::endl;

    delete[] copy;
}

int main(int argc, char** argv) {
    std::cout << "App Started" << std::endl;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".txt") {
            auto arrays = readCArraysFromFile(entry.path().string());
            
            for (size_t i = 0; i < arrays.size(); ++i) {
                std::string benchName = entry.path().filename().string() + "_" + std::to_string(i);
                auto currentArray = arrays[i];
                
                std::cout << "SortV1" << std::endl;
                benchmarkSortV1(benchName, currentArray.data, currentArray.size);
                std::cout << "QuickSort" << std::endl;
                benchmarkQuickSort(benchName, currentArray.data, currentArray.size);
            }

            for (auto& carr : arrays) {
                delete[] carr.data;
            }
        }
    }


    return 0;
}
