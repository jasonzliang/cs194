package math;

public class ConjugateGradientSolver {
	/**
	 * Uses a guess value for x of a zero vector
	 * @param A
	 * @param b
	 * @return
	 */
	public static Vector solve(Matrix A, Vector b) {
		return solve(A, b, new ArrayVector(b.getSize()), 15);
	}

	public static Vector solve(Matrix A, Vector b, Vector x, int maxIters) {
		// r=b-A*x;
		// p=r;
		// rsold=r'*r;
		// for i=1:size(A)
		//   Ap=A*p;
		//   alpha=rsold/(p'*Ap);
		//   x=x+alpha*p;
		//   r=r-alpha*Ap;
		//   rsnew=r'*r;
		//   if sqrt(rsnew)<1e-10
		//     break;
		//   p=r+rsnew/rsold*p;
		//   rsold=rsnew;
		/*
		Vector r = b.vectorSubtraction(x.matrixVectorMultiplication(A));
		Vector p = r.clone();
		float rsold = r.norm();
		Vector Ap;
		float alpha;
		float rsnew;
		for (int i=0; i<maxIters; i++) {
			Ap = p.matrixVectorMultiplication(A);
			alpha = rsold / (p.dotProduct(Ap));
			x.add(p.scalarMultiplication(alpha));
			r.subtract(Ap.scalarMultiplication(alpha));
			rsnew = r.norm();
			if (Math.sqrt(rsnew) < 1e-5) {
				break;
			}
			p = r.vectorAddition(p.scalarMultiplication(rsnew/rsold));
			rsold = rsnew;
			//System.out.println(i + " (" + rsold + "): " + x);
		}
		return x;
		*/
		
		//r = b + A*b;
		float tol = 1e-5f;
		Vector r = b.matrixVectorMultiplication(A);
		r.add(b);
		//y = -r;
		Vector y = r.scalarMultiplication(-1.0f);
		//z = A*y;
		Vector z = y.matrixVectorMultiplication(A);
		//s = y'*z;
		float s = y.dotProduct(z);
		//t = (r'*y)/s;
		float t = r.dotProduct(y)/s;
		//x = -b + t*y;
		x = y.scalarMultiplication(t);
		x.subtract(b);

		//for k = 1:numel(b);
		maxIters = 1000; // requires a ridiculous number of iterations to get anywhere near close
		for (int k=0; k<maxIters; k++) {
			//r = r - t*z;
			r.subtract(z.scalarMultiplication(t));
			//if( norm(r) < tol ) return
			if (r.norm() < tol) {
				break;
			}
			//B = (r'*z)/s;
			float B = r.dotProduct(z) / s;
			//y = -r + B*y;
			y = y.scalarMultiplication(B);
			y.subtract(r);
			//z = A*y;
			z = y.matrixVectorMultiplication(A);
			//s = y'*z;
			s = y.dotProduct(z);
			//t = (r'*y)/s;
			t = r.dotProduct(y) / s;
			//x = x + t*y;
			x.add(y.scalarMultiplication(t));
			//fprintf('Residual: %f\n', norm(A*x-b));
			System.out.println(k + ": " + r.norm() + ", " + x.matrixVectorMultiplication(A).vectorSubtraction(b).norm());
		}
		return x;
	}

	public static Vector pcgSolve(Matrix D, Vector b, Matrix P) {
		Vector x = new ArrayVector(b.getSize());
		Vector r = b.vectorSubtraction(x.matrixVectorMultiplication(D));
		Vector z = r.matrixVectorMultiplication(P);
		Vector p = z.clone();
		Vector cx = x.clone();
		Vector res = b.vectorSubtraction(cx.matrixVectorMultiplication(D));
		int maxIters = 15;
		Vector Ap;
		float v;
		float alpha;
		float beta;
		for (int i=0; i<maxIters; i++) {
			Ap = p.matrixVectorMultiplication(D);
			v = r.dotProduct(z);
			if (Math.abs(v) < 1e-5) {
				break;
			}
			alpha = v / p.dotProduct(Ap);
			cx.add(p.scalarMultiplication(alpha));
			r.subtract(Ap.scalarMultiplication(alpha));
			z = r.matrixVectorMultiplication(P);
			beta = z.dotProduct(r) / v;

			p.multiply(beta);
			p.add(z);

			res = b.vectorSubtraction(cx.matrixVectorMultiplication(D));
		}
		return cx;
	}
}
