cmake_minimum_required(VERSION 3.15)
project(AhoCorasickVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt REQUIRED)

add_executable(aho_corasick source.cpp)

target_link_libraries(aho_corasick PRIVATE fmt::fmt)