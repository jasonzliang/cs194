#run cmake . then make
cmake_minimum_required(VERSION 2.6)
include_directories(.)

# Even though -fopenmp won’t appear in the GUI options, it does take effect
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

add_executable(solver solver.cpp solver.h vector.h matrix.h matrix_market_handler.h array_vector.cpp sparse_hashed_matrix.cpp conjugate_gradient_solver.cpp)

add_executable(driver driver.cpp solver.h vector.h matrix.h matrix_market_handler.h array_vector.cpp sparse_hashed_matrix.cpp conjugate_gradient_solver.cpp)
