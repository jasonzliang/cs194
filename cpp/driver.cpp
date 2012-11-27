#include "conjugate_gradient_solver.cpp"
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
  unsigned int nthreads = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc > 1) {
    nthreads = atoi(argv[1]);
  }

  cout << "Used " << nthreads << " threads" << endl;

  ArrayVector<double> result;

  time_t time_full = time(NULL);
  ConjugateGradientSolver<double>::solve(result, "../matrix.txt.mtx", "../vector.txt.mtx", 1e-6, nthreads, !(argc > 2));

  time_t time_write = time(NULL);
  result.printToMatrixMarketFile("cpp_result.mtx.txt");
  time_write = time(NULL) - time_write;

  time_full = time(NULL) - time_full;

  cout << "Time to write: " << time_write << " [s]" << endl;
  cout << "Full Time: " << time_full << " [s] (may not be the sum of read/solve/write)" << endl;

  return 0;
}
