#run cmake . then make
cmake_minimum_required(VERSION 2.6)
include_directories(.)

# Even though -fopenmp won’t appear in the GUI options, it does take effect
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -O3")

add_executable(llsolver llsolver.cpp)
