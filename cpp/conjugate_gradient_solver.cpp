#include "solver.h"
#include "array_vector.cpp"
#include "sparse_hashed_matrix.cpp"
#include <ctime>
#include <iostream>

#define DEFAULT_TOLERANCE 1e-6

using namespace std;

template <typename T>
class ConjugateGradientSolver : public Solver<T> {
public:
	void static solve(Vector<T> &result, string matrix, string vector, int numThreads, bool verbose) {
		solve(result, matrix, vector, DEFAULT_TOLERANCE, numThreads, verbose);
	}

	void static solve(Vector<T> &result, string matrix, string vector, float tolerance, int numThreads, bool verbose) {

		time_t time_read = time(NULL);
		ArrayVector<T> av;
		av.readFromMatrixMarketFile("../vector.txt.mtx");
		SparseHashedMatrix<T> shm;
		shm.readFromMatrixMarketFile("../matrix.txt.mtx");
		time_read = time(NULL) - time_read;

		time_t time_solve = time(NULL);
		result.resizeVector(av.getSize());
		solve(result, shm, av, tolerance, numThreads, verbose);
		time_solve = time(NULL) - time_solve;

		cout << "Time to read: " << time_read << " [s]" << endl;
		cout << "Time to solve: " << time_solve << " [s]" << endl;
	}

	void static solve(Vector<T> &result, Matrix<T> &matrix, Vector<T> &vector, int numThreads, bool verbose) {
		solve(result, matrix, vector, DEFAULT_TOLERANCE, numThreads, verbose);
	}

	// note: x = result
	void static solve(Vector<T> &x, Matrix<T> &A, Vector<T> &b, float tolerance, int numThreads, bool verbose) {
		ArrayVector<T> r(b.getSize());
		ArrayVector<T> y(b.getSize());
		ArrayVector<T> z(b.getSize());
		T s;
		T t;
		ArrayVector<T> temp(b.getSize());

		//r = b + A*b;
		A.multiply(b, r, numThreads);
		r.increaseBy(b);
		//y = -r;
		r.multiply(-1.0, y);
		//z = A*y;
		A.multiply(y, z, numThreads);
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

			if (verbose) {
				cout << "Residual (" << k << "/" << maxIters << "): " << r.norm() << endl;
			}

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
			A.multiply(y, z, numThreads);
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
