cmake_minimum_required(VERSION 3.20.0)
project(KOKESortTest VERSION 0.1.0 LANGUAGES C)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH GLOBAL_SRC) #CMake 3.20 Feature

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_link_libraries(${TEST_NAME} KOKESort)

    target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../parts)

    #set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

    add_custom_command(TARGET ${TEST_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:KOKESort> $<TARGET_FILE_DIR:${TEST_NAME}>)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    set_tests_properties(${TEST_NAME} PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endforeach()