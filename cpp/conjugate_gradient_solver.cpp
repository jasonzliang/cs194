#include "solver.h"
#include "array_vector.cpp"
#include "sparse_hashed_matrix.cpp"

#define DEFAULT_TOLERANCE 1e-6

template <typename T>
class ConjugateGradientSolver : public Solver<T> {
  public:
  void static solve(Vector<T> &result, string matrix, string vector) {
    solve(result, matrix, vector, DEFAULT_TOLERANCE);
  }

  void static solve(Vector<T> &result, string matrix, string vector, float tolerance) {
    ArrayVector<T> av;
    av.readFromMatrixMarketFile("../vector.txt.mtx");
    SparseHashedMatrix<T> shm;
    shm.readFromMatrixMarketFile("../matrix.txt.mtx");
    result.resizeVector(av.getSize());
    solve(result, shm, av, tolerance);
  }

  void static solve(Vector<T> &result, Matrix<T> &matrix, Vector<T> &vector) {
    solve(result, matrix, vector, DEFAULT_TOLERANCE);
  }

  // note: x = result
  void static solve(Vector<T> &x, Matrix<T> &A, Vector<T> &b, float tolerance) {
    ArrayVector<T> r(b.getSize());
    ArrayVector<T> y(b.getSize());
    ArrayVector<T> z(b.getSize());
    T s;
    T t;
    ArrayVector<T> temp(b.getSize());

    //r = b + A*b;
    A.multiply(b, r);
    r.increaseBy(b);
    //y = -r;
    r.multiply(-1, x);
    //z = A*y;
    A.multiply(y, z);
    //s = y'*z;
    s = y.dotProduct(z);
    //t = (r'*y)/s;
    t = r.dotProduct(y) / s;
    //x = -b + t*y;
    y.multiply(t, x);
    x.reduceBy(b);

    //for k = 1:numel(b);
    int maxIters = b.getSize();
    for (int k=0; k<maxIters; k++) {
      // r = r - t*z;
      z.multiply(t, temp);
      r.reduceBy(temp);

      cout << "Residual (" << k << "/" << maxIters << "): " << r.norm() << endl;

      // if( norm(r) < tol )
      //	  return;
      // end
      if (r.norm() < tolerance) {
	break;
      }
      // B = (r'*z)/s;
      T B = r.dotProduct(z) / s;
      // y = -r + B*y;
      y.scaleBy(B);
      y.reduceBy(r);
      // z = A*y;
      A.multiply(y, z);
      // s = y'*z;
      s = y.dotProduct(z);
      // t = (r'*y)/s;
      t = r.dotProduct(y) / s;
      // x = x + t*y;
      y.multiply(t, temp);
      x.increaseBy(temp);
      // fprintf('Residual: %f\n', norm(A*x-b));
    //end
    }
  }
};
