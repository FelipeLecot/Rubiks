cmake_minimum_required(VERSION 3.16)
project(rubiks CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(src)

foreach(demo embedded parity bigcube graph debug)
    add_executable(${demo} src/demos/${demo}.cpp)
endforeach()

find_package(OpenMP)
add_executable(ll_optimal src/tools/ll_optimal.cpp)
if(OpenMP_CXX_FOUND)
    target_link_libraries(ll_optimal PRIVATE OpenMP::OpenMP_CXX)
endif()
