cmake_minimum_required(VERSION 3.10)

project(Rope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Rope src/main.cpp src/rope.cpp)
target_include_directories(Rope PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Needs to be in top-level CMakeLists
enable_testing()

add_subdirectory(test)
