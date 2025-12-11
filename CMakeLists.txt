cmake_minimum_required(VERSION 3.26)

project(HoPiler LANGUAGES CXX)

# Enable C++20 (or whatever you use)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Collect all .cpp files in src/
file(GLOB SRC_FILES src/*.cpp)

add_executable(HoPiler ${SRC_FILES})
