# Testleri derliyoruz

#[[
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/FindInsertIndexBSTest.c)

# Teste kitaplığı linkliyoruz
target_link_libraries(Test KOKESort)

target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})

# Testleri çalıştırılabilir hale getiriyoruz
add_test(NAME Test COMMAND Test)

set_tests_properties(Test PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
]]

#[[
cmake_minimum_required(VERSION 3.20)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

# Mull ve LLVM yolları
set(MULL_DIR "${CMAKE_SOURCE_DIR}/tools/mull")
set(LLVM_CONFIG "${MULL_DIR}/llvm-config")
set(CMAKE_CXX_COMPILER "${MULL_DIR}/clang++")

# Bitcode için gerekli bayraklar
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-instr-generate -fcoverage-mapping -emit-llvm -c")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Bitcode dosyalarının listesi
file(GLOB_RECURSE BITCODE_FILES "${CMAKE_BINARY_DIR}/*.bc")
file(WRITE ${CMAKE_BINARY_DIR}/bitcode_files.txt "${BITCODE_FILES}")



#Automated Test File Detecting
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} KOKESort)
    target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach()

]]

#set(CMAKE_CXX_STANDARD 20)

#[[
# Mull Config

# Setting Compilers
set(MULL_DIR "${CMAKE_SOURCE_DIR}/tools/mull")
set(LLVM_CONFIG "${MULL_DIR}/llvm-config")
set(CMAKE_CXX_COMPILER "${MULL_DIR}/clang++")

# Bitcode Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -emit-llvm -c")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
]]

# Automated test dosyalarını bulma
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} KOKESort)
    target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})

    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

    #[[ 
    # Mull Config
    target_compile_options(${TEST_NAME} PRIVATE -emit-llvm -c)
    ]]

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach()

#[[
# Mull Config
file(GLOB_RECURSE BITCODE_FILES "${CMAKE_BINARY_DIR}/*.bc")
file(WRITE ${CMAKE_BINARY_DIR}/bitcode_files.txt "${BITCODE_FILES}")

# Debug: Bitcode dosyalarını kontrol et
message(STATUS "Bitcode files found: ${BITCODE_FILES}")
]]