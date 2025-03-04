cmake_minimum_required(VERSION 3.5.0) #Not Tested
project(KOKESort VERSION 0.1.0 LANGUAGES C)

#[[
# For Mull Usage
set(CMAKE_CXX_COMPILER "${MULL_DIR}/clang++")
set(CMAKE_C_COMPILER "${CMAKE_SOURCE_DIR}/tools/mull/clang")
]]

if(USE_GENERATOR)
# ➤ Fonksiyonları ve parametrelerini tanımla
set(FUNCTIONS
    "Int|int"
    "Long|long"
)

# ➤ Çıktı dizinini belirle
set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/src/generated")

if(EXISTS ${GENERATED_DIR})
    file(REMOVE_RECURSE ${GENERATED_DIR})
endif()
file(MAKE_DIRECTORY ${GENERATED_DIR})

# ➤ Header dosyaları için include listesini oluştur
set(INCLUDE_LIST "")

# ➤ Fonksiyonları üret
foreach(FUNC_DEF ${FUNCTIONS})
    string(REPLACE "|" ";" FUNC_PARTS ${FUNC_DEF})
    list(GET FUNC_PARTS 0 FUNC_NAME)
    list(GET FUNC_PARTS 1 FUNC_TYPE)

    configure_file(${CMAKE_SOURCE_DIR}/templates/TypeBasedGenerator.c.in ${GENERATED_DIR}/${FUNC_NAME}.c @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/templates/TypeBasedGenerator.h.in ${GENERATED_DIR}/${FUNC_NAME}.h @ONLY)
endforeach()

# ➤ Kaynak dosyaları topla
file(GLOB GENERATED_SOURCES "${GENERATED_DIR}/*.c")

# ➤ Kütüphaneyi oluştur
add_library(KOKESort KOKESort.c ${GENERATED_SOURCES})

# ➤ Include dizinlerini ekle
target_include_directories(KOKESort PUBLIC src ${GENERATED_DIR})
else()
# ➤ Kütüphaneyi oluştur
add_library(KOKESort KOKESort.c)

# ➤ Include dizinlerini ekle
target_include_directories(KOKESort PUBLIC src)
endif()

#[[
# Mutation Test Symbols
target_compile_options(KOKESort PRIVATE -emit-llvm -c)
set_target_properties(KOKESort PROPERTIES OUTPUT_NAME "KOKESort.bc")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

message(STATUS "CMake compiler: ${CMAKE_CXX_COMPILER}")
# Mutation Test Symbols
]]

include(CTest)
enable_testing()
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
