cmake_minimum_required(VERSION 4.0)
project(CPPProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPProject main.cpp
        8_Union_C/8_Union.h
        8_Union_C/8_Union.c
        8_Union_C/8_Union_test.c
        8_Union_CPP/8_Union.h
        8_Union_CPP/8_Union.cpp
        8_Union_CPP/8_Union_test.cpp)
