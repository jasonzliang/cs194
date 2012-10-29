package math;

public class ConjugateGradientSolver {
	/**
	 * Uses a guess value for x of a zero vector
	 * @param A
	 * @param b
	 * @return
	 */
	public static Vector solve(Matrix A, Vector b) {
		return solve(A, b, new Vector(b.getSize()), 15);
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
			if (Math.sqrt(rsnew) < 1e-10) {
				break;
			}
			p = r.vectorAddition(p.scalarMultiplication(rsnew/rsold));
			rsold = rsnew;
			//System.out.println(i + " (" + rsold + "): " + x);
		}
		return x;
	}
}
