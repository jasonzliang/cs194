#include "conjugate_gradient_solver.cpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  cout << "hey hey" << endl;

  ArrayVector<double> result;
  ConjugateGradientSolver<double>::solve(result, "../matrix.txt.mtx", "../vector.txt.mtx", 1e-6);

  return 0;
}
