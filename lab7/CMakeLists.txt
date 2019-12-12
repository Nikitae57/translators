cmake_minimum_required(VERSION 3.15)
project(lab7)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC
    "*.h"
    "*.cpp"
)


add_executable(lab7 ${SRC})
