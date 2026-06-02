cmake_minimum_required(VERSION 3.16)
project(rubiks CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(src)

foreach(demo embedded parity bigcube graph debug)
    add_executable(${demo} src/demos/${demo}.cpp)
endforeach()
