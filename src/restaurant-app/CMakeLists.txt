cmake_minimum_required(VERSION 3.21)

project(RestaurantApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

CPMAddPackage(
  NAME FluentUI
  GIT_REPOSITORY https://github.com/mowangshuying/FluentUI.git
  GIT_TAG master
)

add_subdirectory(${FluentUI_SOURCE_DIR} ${FluentUI_BINARY_DIR})

add_executable(RestaurantApp main.cpp)

target_link_libraries(RestaurantApp PRIVATE Qt6::Quick)
target_link_libraries(RestaurantApp FluentUI)
