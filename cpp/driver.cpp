#include "conjugate_gradient_solver.cpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

  ArrayVector<double> result;
  ConjugateGradientSolver<double>::solve(result, "../matrix.txt.mtx", "../vector.txt.mtx", 1e-6);

  result.printToMatrixMarketFile("cpp_result.mtx.txt");

  return 0;
}
