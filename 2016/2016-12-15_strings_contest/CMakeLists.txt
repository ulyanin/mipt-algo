cmake_minimum_required(VERSION 3.6)
project(2016_12_15_strings_contest)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOCAL -Wall")
set(SOURCE_FILES task_j_trie.cpp)
add_executable(2016_12_15_strings_contest ${SOURCE_FILES})