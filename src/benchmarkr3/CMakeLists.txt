cmake_minimum_required(VERSION 3.18)
project(KOKESortBenchmarkR3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

CPMAddPackage(
  NAME cpp_sort
  GIT_REPOSITORY https://github.com/Morwenn/cpp-sort.git
  GIT_TAG 1.16.0
)

CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  VERSION 3.12.0
  OPTIONS
        "JSON_BuildTests OFF"
)
add_executable(KOKESortBenchmarkR3 main.cpp)
target_link_libraries(KOKESortBenchmarkR3 KOKESort cpp-sort nlohmann_json::nlohmann_json)

#[[
if(USE_GOOGLE_BENCHMARK)
target_link_libraries(KOKESortBenchmarkR2 benchmark::benchmark)
endif()
]]

target_include_directories(KOKESortBenchmarkR3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../parts ${cpp_sort_INCLUDE_DIRS} ${nlohmann_json_INCLUDE_DIRS})