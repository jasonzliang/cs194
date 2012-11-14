#run cmake . then make
cmake_minimum_required(VERSION 2.6)
include_directories(.)
<<<<<<< HEAD
add_executable(solver solver.cpp solver.h vector.h matrix.h matrix_market_handler.h array_vector.cpp)
=======
add_executable(solver solver.cpp solver.h vector.h matrix.h printable_element_container.h array_vector.cpp sparse_hashed_matrix.cpp)
>>>>>>> 515ca5d878acaa61e43055ada10fdf423e6a6008
