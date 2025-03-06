cmake_minimum_required(VERSION 3.18)
project(KOKESortBenchmark)

add_executable(KOKESortBenchmark main.c)
target_link_libraries(KOKESortBenchmark KOKESort)

target_include_directories(KOKESortBenchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})