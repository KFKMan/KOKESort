cmake_minimum_required(VERSION 3.18)
project(KOKESortWrapper)

#[[
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
)
FetchContent_MakeAvailable(pybind11)
]]

##CPMAddPackage("gh:pybind/pybind11@2.13.6")

#find_package(pybind11 REQUIRED)

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

find_package(Python COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)

message(STATUS "Python_EXECUTABLE: ${Python_EXECUTABLE}")
message(STATUS "nanobind_ROOT: ${nanobind_ROOT}")

  list(APPEND CMAKE_PREFIX_PATH ${nanobind_ROOT})

set(nanobind_DIR ${nanobind_ROOT})

find_package(nanobind CONFIG REQUIRED)

#find_package(Python3 COMPONENTS Interpreter Development)

#add_library(KOKESortWrapper MODULE wrapper.cpp)

#pybind11_add_module(KOKESortWrapper src/wrapper.cpp)

nanobind_add_module(KOKESortWrapper src/wrapper.cpp)

target_compile_definitions(KOKESortWrapper PRIVATE KOKESORT_STATIC)

#target_link_libraries(KOKESortWrapper PRIVATE pybind11::module)
target_link_libraries(KOKESortWrapper PRIVATE KOKESort)


target_include_directories(KOKESortWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_SOURCE_DIR} ${pybind11_INCLUDE_DIRS})