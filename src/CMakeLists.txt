cmake_minimum_required(VERSION 3.20.0)
project(KOKESort VERSION 0.1.0 LANGUAGES C)

option(PYBINDWRAPPER "Adding pybindwrapper to list" OFF)

option(USE_TESTS "Using tests" ON)

option(USE_BENCHMARKS "Using bechmarks" OFF)

option(USE_GENERATOR "Use generators" ON)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    #set(CMAKE_BUILD_TYPE Debug)

    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Coverage enabled: Forcing Debug build type")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -O0 -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
    else()
        message(WARNING "Coverage is only supported for GCC and Clang.")
    endif()
endif()

if(WIN32)
    set(LIB_TYPE SHARED) # For Windows create DLL
else()
    set(LIB_TYPE STATIC) # For others create LIB
endif()

set(SRC_FILES KOKESort.c)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

if(USE_GENERATOR)
# ➤ Fonksiyonları ve parametrelerini tanımla
set(FUNCTIONS
    "Int|int"
    "Long|long"
    "Float|float"
    "Double|double"
)

# ➤ Çıktı dizinini belirle
set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/generated")

if(EXISTS ${GENERATED_DIR})
    file(REMOVE_RECURSE ${GENERATED_DIR})
endif()
file(MAKE_DIRECTORY ${GENERATED_DIR})

# ➤ Fonksiyonları üret
foreach(FUNC_DEF ${FUNCTIONS})
    string(REPLACE "|" ";" FUNC_PARTS ${FUNC_DEF})
    list(GET FUNC_PARTS 0 FUNC_NAME)
    list(GET FUNC_PARTS 1 FUNC_TYPE)

    configure_file(${CMAKE_SOURCE_DIR}/templates/TypeBasedGenerator.c.in ${GENERATED_DIR}/Sort${FUNC_NAME}.c @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/templates/TypeBasedGenerator.h.in ${GENERATED_DIR}/Sort${FUNC_NAME}.h @ONLY)
endforeach()

set(ALL_INCLUDES "")
foreach(FUNC_DEF ${FUNCTIONS})
    string(REPLACE "|" ";" FUNC_PARTS ${FUNC_DEF})
    list(GET FUNC_PARTS 0 FUNC_NAME)
    list(GET FUNC_PARTS 1 FUNC_TYPE)

    set(ALL_INCLUDES "${ALL_INCLUDES}\n#include \"Sort${FUNC_NAME}.h\"")
endforeach()

configure_file(${CMAKE_SOURCE_DIR}/templates/MasterHeader.h.in ${GENERATED_DIR}/MasterHeader.h)

# ➤ Kaynak dosyaları topla
file(GLOB GENERATED_SOURCES "${GENERATED_DIR}/*.c")

list(APPEND SRC_FILES ${GENERATED_SOURCES})
list(APPEND INCLUDE_DIRS ${GENERATED_DIR})
endif()

if(USE_TESTS)
list(APPEND SRC_FILES csv.c)
endif()

add_library(KOKESort ${LIB_TYPE} ${SRC_FILES})
target_include_directories(KOKESort PUBLIC ${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set_target_properties(KOKESort PROPERTIES POSITION_INDEPENDENT_CODE ON)

#target_compile_definitions(KOKESort PRIVATE KOKESORT_IMPORTS)
target_compile_definitions(KOKESort PRIVATE KOKESORT_EXPORTS)

if(USE_TESTS)
include(CTest)
enable_testing()
add_subdirectory(tests)
endif()

if(PYBINDWRAPPER)
add_subdirectory(pybindwrapper)
endif()

if(USE_BENCHMARKS)
add_subdirectory(benchmark)
endif()