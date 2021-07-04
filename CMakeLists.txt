cmake_minimum_required(VERSION 3.19)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp memory.h memory.cpp order.h order.cpp CPU.h CPU.cpp)