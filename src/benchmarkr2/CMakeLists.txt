cmake_minimum_required(VERSION 3.18)
project(KOKESortBenchmarkR2)

#[[
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_GOOGLE_BENCHMARK off)

if(USE_GOOGLE_BENCHMARK)
CPMAddPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    VERSION 1.9.1
    OPTIONS
        "BENCHMARK_DOWNLOAD_DEPENDENCIES On"
        "CMAKE_BUILD_TYPE Release"
)
endif()
]]

#add_executable(KOKESortBenchmarkR2 main.cpp)
add_executable(KOKESortBenchmarkR2 main.c file.c advtime.c)
target_link_libraries(KOKESortBenchmarkR2 KOKESort)

#[[
if(USE_GOOGLE_BENCHMARK)
target_link_libraries(KOKESortBenchmarkR2 benchmark::benchmark)
endif()
]]

target_include_directories(KOKESortBenchmarkR2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../parts)