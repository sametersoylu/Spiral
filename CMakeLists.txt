cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME "Spiral")

project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
find_package(Gorgon REQUIRED)

add_executable(Spiral main.cpp Spiral.cpp)

linkgorgon(${PROJECT_NAME})

