cmake_minimum_required(VERSION 3.15)
project(lab8)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC
    "*.h"
    "*.cpp"
)


add_executable(lab8 ${SRC} TokenListBuilder.cpp TokenListBuilder.h AstBuilder.cpp AstBuilder.h)
