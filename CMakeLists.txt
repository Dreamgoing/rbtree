cmake_minimum_required(VERSION 3.6)
project(rbtree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp RBtreeNode.cpp RBtreeNode.h RBtree.h)
add_executable(rbtree ${SOURCE_FILES})