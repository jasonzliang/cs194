#ifndef SOLVER_H
#define SOLVER_H

#include "vector.h"
#include "matrix.h"

template <typename T>
class Solver {
  public:
  void static solve(Vector<T> result, string, string, int, bool);
  void static solve(Vector<T> result, string, string, float, int, bool);
  void static solve(Vector<T> result, Matrix<T>, Vector<T>, int, bool);
  void static solve(Vector<T> result, Matrix<T>, Vector<T>, float, int, bool);  
};

#endif
