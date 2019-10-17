cmake_minimum_required(VERSION 3.15)
project(lab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab2 main.cpp structs.h LinkedList.h)

target_link_libraries(lab2
#    -Wl,-rpath=${LINK_DIR_REL}
    -Wl,-z,defs
    -Wl,-z,now
    stdc++
)