#include "solver.h"
#include "array_vector.cpp"
#include "sparse_hashed_matrix.cpp"

#define DEFAULT_TOLERANCE 1e-6

template <typename T>
class ConjugateGradientSolver : public Solver<T> {
  public:
  void static solve(Vector<T> result, string matrix, string vector) {
    solve(result, matrix, vector, DEFAULT_TOLERANCE);
  }

  void static solve(Vector<T> result, string matrix, string vector, float tolerance) {
    ArrayVector<T> av;
    av.readFromMatrixMarketFile("../vector.txt.mtx");
    SparseHashedMatrix<T> shm;
    shm.readFromMatrixMarketFile("../matrix.txt.mtx");
    result.resizeVector(av.getSize());
    solve(result, shm, av, tolerance);
  }

  void static solve(Vector<T> result, Matrix<T> matrix, Vector<T> vector) {
    solve(result, matrix, vector, DEFAULT_TOLERANCE);
  }

  void static solve(Vector<T> x, Matrix<T> A, Vector<T> b, float tolerance) {
    ArrayVector<T> r;
    ArrayVector<T> y;
    ArrayVector<T> z;
    T s;
    T t;

    //r = b + A*b;
    A.multiply(b, r);
    r.increaseBy(b);
    //y = -r;
    r.multiply(-1, x);
    

    int maxIters = b.getSize();
  }
};
