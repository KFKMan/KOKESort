cmake_minimum_required(VERSION 3.18)
project(KOKESortWrapper)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
)
FetchContent_MakeAvailable(pybind11)

#find_package(pybind11 REQUIRED)

find_package(Python3 COMPONENTS Interpreter Development)

#add_library(KOKESortWrapper MODULE wrapper.cpp)
pybind11_add_module(KOKESortWrapper src/wrapper.cpp)

target_compile_definitions(KOKESortWrapper PRIVATE KOKESORT_IMPORTS)

target_link_libraries(KOKESortWrapper PRIVATE pybind11::module)
target_link_libraries(KOKESortWrapper PRIVATE KOKESort)


target_include_directories(KOKESortWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_SOURCE_DIR} ${pybind11_INCLUDE_DIRS})