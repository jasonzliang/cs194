#ifndef SOLVER_H
#define SOLVER_H

#include "vector.h"
#include "matrix.h"

template <typename T>
class Solver {
  public:
  void static solve(Vector<T> result, string, string);
  void static solve(Vector<T> result, string, string, float);
  void static solve(Vector<T> result, Matrix<T>, Vector<T>);
  void static solve(Vector<T> result, Matrix<T>, Vector<T>, float);  
};

#endif
