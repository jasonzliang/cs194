#run cmake . then make
cmake_minimum_required(VERSION 2.6)
include_directories(.)
add_executable(solver solver.cpp solver.h vector.h matrix.h matrix_market_handler.h array_vector.cpp sparse_hashed_matrix.cpp)
