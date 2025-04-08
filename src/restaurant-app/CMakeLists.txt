cmake_minimum_required(VERSION 3.21)

project(RestaurantApp LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Tools)

add_executable(RestaurantApp main.cpp)

target_link_libraries(RestaurantApp PRIVATE Qt6::Widgets)