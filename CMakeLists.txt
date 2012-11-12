#run cmake . then make
cmake_minimum_required(VERSION 2.6)
include_directories(.)
add_executable(solver solver.cpp solver.h vector.h matrix.h)
