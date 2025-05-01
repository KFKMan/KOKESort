cmake_minimum_required(VERSION 3.20.0)
project(KOKESort VERSION 0.1.0 LANGUAGES C)

option(PYBINDWRAPPER "Adding pybindwrapper to list" OFF)

option(USE_TESTS "Using tests" ON)

option(USE_GENERATOR "Use generators" ON)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

option(USE_BENCHMARKS "Using bechmarks" OFF)

option(USE_BENCHMARKS_R2 "Using advanced benchmarks" OFF)

option(USE_BENCHMARKS_R3 "Using our most advanced benchmark mechanism" OFF)

option(USE_PARTS "Use other sorting algorithms" ON) #Don't change it, currently not supporting OFF parameter

option(GENERATE_DOCUMENTATION "Generate documentation via Doxygen" OFF)

option(GENERATE_DOCUMENTATION_CPM "Generate documentation via Doxygen - CPM (Not Working Currently)" OFF)

option(USE_RESTRAURANT_APP "Build Restaurant App" OFF)

option(USE_MUTATION_TESTS "Using mutation tests" OFF)

message("C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message("CXX Compiler ID: ${CMAKE_C_COMPILER_ID}")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
include(get_cpm)

set(TOOLS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tools)

if(USE_RESTRAURANT_APP)
    message("Adding VCPKG")

    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" STRING "Vcpkg toolchain file")
endif()

if (GENERATE_DOCUMENTATION)

    if(GENERATE_DOCUMENTATION_CPM)
        CPMAddPackage(
            NAME Doxygen
            GITHUB_REPOSITORY "doxygen/doxygen"
            VERSION 1.13.2
            OPTIONS "DOXYGEN_BUILD_DOC OFF"
        )

        if (Doxygen_ADDED)
            set(DOXYGEN_EXECUTABLE "${Doxygen_BINARY_DIR}/bin/doxygen")
        else()
            message(WARNING "Doxygen (CPM) can't added.")
            set(GENERATE_DOCUMENTATION OFF)
        endif()
    else()
        find_package(Doxygen)

        if (DOXYGEN_FOUND)
            set(DOXYGEN_EXECUTABLE ${DOXYGEN_EXECUTABLE})
        else()
            message(WARNING "Doxygen not found")
            set(GENERATE_DOCUMENTATION OFF)
        endif()
    endif()
endif()

if (GENERATE_DOCUMENTATION AND DOXYGEN_EXECUTABLE)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    if (EXISTS ${DOXYFILE_IN})
        add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating Doxygen Documnetation..."
            VERBATIM
        )
    else()
        message(WARNING "Doxyfile not found: ${DOXYFILE_IN}")
    endif()

endif()


if (USE_BENCHMARKS OR USE_BENCHMARKS_R2)
    message(STATUS "Release build with maximum optimization")
    if (MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
        set(CMAKE_C_FLAGS_RELEASE "/O2 /DNDEBUG")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    endif()
endif()

if(ENABLE_COVERAGE)
    #set(CMAKE_BUILD_TYPE Debug)

    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Coverage enabled: Forcing Debug build type")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    endif()

    #More control flag can be good.
    if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU") 
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -O0 -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
        message(WARNING "Optimization Level Setted To O0 For Coverage")
    else()
        message(WARNING "Coverage is only supported for GCC and Clang.")
    endif()
endif()

if(WIN32)
    set(LIB_TYPE SHARED) # For Windows create DLL
else()
    set(LIB_TYPE STATIC) # For others create LIB
endif()

set(SRC_FILES KOKESort.c KOKESortV2.c Global.c csv.c)
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
set(GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

if(EXISTS ${GENERATED_DIR})
    file(REMOVE_RECURSE ${GENERATED_DIR})
endif()
file(MAKE_DIRECTORY ${GENERATED_DIR})

# ➤ Fonksiyonları üret
foreach(FUNC_DEF ${FUNCTIONS})
    string(REPLACE "|" ";" FUNC_PARTS ${FUNC_DEF})
    list(GET FUNC_PARTS 0 FUNC_NAME)
    list(GET FUNC_PARTS 1 FUNC_TYPE)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/templates/TypeBasedGenerator.c.in ${GENERATED_DIR}/Sort${FUNC_NAME}.c @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/templates/TypeBasedGenerator.h.in ${GENERATED_DIR}/Sort${FUNC_NAME}.h @ONLY)
endforeach()

set(ALL_INCLUDES "")
foreach(FUNC_DEF ${FUNCTIONS})
    string(REPLACE "|" ";" FUNC_PARTS ${FUNC_DEF})
    list(GET FUNC_PARTS 0 FUNC_NAME)
    list(GET FUNC_PARTS 1 FUNC_TYPE)

    set(ALL_INCLUDES "${ALL_INCLUDES}\n#include \"Sort${FUNC_NAME}.h\"")
endforeach()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/templates/MasterHeader.h.in ${GENERATED_DIR}/MasterHeader.h)

# ➤ Kaynak dosyaları topla
file(GLOB GENERATED_SOURCES "${GENERATED_DIR}/*.c")

list(APPEND SRC_FILES ${GENERATED_SOURCES})
list(APPEND INCLUDE_DIRS ${GENERATED_DIR})
endif()

if(USE_TESTS)
#list(APPEND SRC_FILES csv.c)
endif()

if(USE_PARTS)
set(PARTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/parts")
file(GLOB PARTS_SOURCES "${PARTS_DIR}/*.c")
list(APPEND SRC_FILES ${PARTS_SOURCES})
list(APPEND INCLUDE_DIRS ${PARTS_DIR})
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

if(USE_BENCHMARKS_R2)
add_subdirectory(benchmarkr2)
endif()

if(USE_BENCHMARKS_R3)
add_subdirectory(benchmarkr3)
endif()

if(USE_RESTRAURANT_APP)
message(INFO "Adding Restaurant App")
add_subdirectory(restaurant-app)
message(INFO "Restaurant App Added")
endif()