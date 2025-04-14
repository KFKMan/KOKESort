#include "sortsys.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <nlohmann/json.hpp>
#include <random>

#include <sortsys.hpp>
#include <cwrappersys.hpp>

using json = nlohmann::json;

std::vector<int> generate_data(size_t size)
{
    std::vector<int> vec(size);
    std::iota(vec.begin(), vec.end(), 0);
    std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});
    return vec;
}

template <typename Sorter>
void benchmark_sort(const std::vector<int> &input, Sorter sorter, const std::string &sorter_name, size_t size, const std::string &filename)
{
    std::vector<int> data = input; // Verinin bir kopyasını al
    auto start = std::chrono::high_resolution_clock::now();
    sorter(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Sonucu CSV dosyasına kaydet
    std::ofstream file(filename, std::ios_base::app);
    if (file.is_open())
    {
        file << sorter_name << "," << size << "," << duration.count() << "\n";
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
    }
}

void initialize_csv(const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << "Algorithm,Size,Time\n";
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not create file!" << std::endl;
    }
}

typedef int GenerateValueType;

const int MaxValue = 10000000;

const int SpaceCount = 100;

GenerateValueType Divider = MaxValue / SpaceCount;

size_t indexer(const void *valptr)
{
    const GenerateValueType val1 = *(const GenerateValueType *)valptr;
    return val1 / Divider;
}

template <typename T>
std::vector<T> random_vector(size_t size, T min, T max)
{
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dis(min, max);

    std::vector<T> data(size);
    for (size_t i = 0; i < size; ++i)
    {
        data[i] = dis(gen);
    }

    return data;
}

int main()
{
    std::ifstream file("test.json");
    if (!file)
    {
        std::cerr << "test.json not found!\n";
        return 1;
    }

    json config;
    file >> config;

    auto algorithm_map = get_sorters();

    auto algorithms = config["enabled_sorters"].get<std::vector<std::string>>();
    auto sizes = config["dataset_size"].get<std::vector<size_t>>();
    size_t repeat_size = config.value("repeat_size", 3);
    size_t repeat_set = config.value("repeat_dataset", 3);

    const std::string filename = "benchmark_results.csv";
    initialize_csv(filename);

    std::map<std::string, SorterFunction> enabledFunctions;

    for (const auto &algo_name : algorithms)
    {
        if (algorithm_map.find(algo_name) != algorithm_map.end())
        {
            enabledFunctions[algo_name] = algorithm_map[algo_name];
        }
    }

    for (size_t size : sizes)
    {
        // Repeat Per Dataset Size
        for (size_t dataset_idx = 0; dataset_idx < repeat_size; ++dataset_idx)
        {
            std::vector<int> data = random_vector<GenerateValueType>(size, 0, MaxValue);

            for (const auto &algo_name : algorithms)
            {
                auto algo = algorithm_map.find(algo_name);
                if (algo != algorithm_map.end())
                {
                    // Repeat Per Same Dataset
                    for (size_t repeat = 0; repeat < repeat_set; ++repeat)
                    {
                        std::cout << "Benchmarking " << algo_name << " on " << size << " elements (Repeat " << repeat + 1 << " of Dataset " << dataset_idx + 1 << ")" << std::endl;
                        benchmark_sort(data, algo->second, algo_name, size, filename);
                    }
                }
            }
        }
    }

    return 0;
}
