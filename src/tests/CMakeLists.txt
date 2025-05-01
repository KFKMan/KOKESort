cmake_minimum_required(VERSION 3.20.0)
project(KOKESortTest VERSION 0.1.0 LANGUAGES C)

message(STATUS "Working on tests...")

set(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

if(USE_MUTATION_TESTS)
message(STATUS "Mutation Tests Enabled")

set(TARGET_DIR "${TARGET_DIR}/mutated")
message(STATUS "Target Directory Setted to ${TARGET_DIR}")
#file(REMOVE_RECURSE "${TARGET_DIR}/")
message(STATUS "Target Directory Cleaned for New Mutations")

make_directory("${TARGET_DIR}")

set(MUTATOR_EXECUTABLE_PATH "${TOOLS_PATH}/KOKEMutator.exe")
message(STATUS "Mutator Executable Setted to ${MUTATOR_EXECUTABLE_PATH}")

message(STATUS "Running Mutator...")
add_custom_target(
    run_mutator_with_params_external
    COMMAND ${MUTATOR_EXECUTABLE_PATH} -d ${CMAKE_CURRENT_SOURCE_DIR} -o ${TARGET_DIR}/{name}{index}{ext}
    COMMENT "Running mutation tool with -d ${CMAKE_CURRENT_SOURCE_DIR} -o ${TARGET_DIR}/{name}{index}{ext}"
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
endif()

file(GLOB TEST_SOURCES "${TARGET_DIR}/*.c")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH GLOBAL_SRC) #CMake 3.20 Feature

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_link_libraries(${TEST_NAME} KOKESort)

    target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../parts)

    #set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

    add_custom_command(TARGET ${TEST_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:KOKESort> $<TARGET_FILE_DIR:${TEST_NAME}>)

    if(USE_MUTATION_TESTS)
    add_test(NAME ${TEST_NAME}
             COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/mutant_wrapper.py $<TARGET_FILE:${TEST_NAME}>)
    else()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endif()

    set_tests_properties(${TEST_NAME} PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endforeach()