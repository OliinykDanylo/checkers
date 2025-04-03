cmake_minimum_required(VERSION 3.29)
project(Checkers_C__)

set(CMAKE_CXX_STANDARD 20)

add_library(Checkers_C__ SHARED library.cpp
        Main.h
        Main.cpp
        CheckersGame.h
        CheckersGame.cpp)

#target_link_options(Checkers_C__ PRIVATE "-static")

target_include_directories(Checkers_C__ PRIVATE "/Library/Java/JavaVirtualMachines/jdk-21.jdk/Contents/Home/include")
target_include_directories(Checkers_C__ PRIVATE "/Library/Java/JavaVirtualMachines/jdk-21.jdk/Contents/Home/include/darwin")