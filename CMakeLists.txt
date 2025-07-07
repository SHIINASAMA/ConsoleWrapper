cmake_minimum_required(VERSION 3.10)

project(ConsoleWrapper)
set(CMAKE_CXX_STANDARD 17)

add_executable(
        ConsoleWrapper
        src/Config.h
        src/Main.h
        src/Main.cpp
)