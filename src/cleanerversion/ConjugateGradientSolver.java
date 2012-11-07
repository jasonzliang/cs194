package cleanerversion;

public class ConjugateGradientSolver {
	public static Vector solve(Matrix A, Vector b) {
		return solve(A, b, 1e-10f);
	}

	public static Vector solve(Matrix A, Vector b, float tolerance) {
	    //r = b + A*b;
		Vector r = A.multiply(b).increaseBy(b);
	    //y = -r;
		Vector y = r.multiply(-1.0f);
	    //z = A*y;
		Vector z = A.multiply(y);
	    //s = y'*z;
		float s = y.dotProduct(z);
	    //t = (r'*y)/s;
		float t = r.dotProduct(y) / s;
	    //x = -b + t*y;
		Vector x = y.multiply(t).subtractBy(b);

	    //for k = 1:numel(b);
		for (int k=0; k<b.getSize(); k++) {
			// r = r - t*z;
			r.subtractBy(z.multiply(t));
			// if( norm(r) < tol )
			//      return;
			// end
			if (r.norm() < tolerance) {
				break;
			}
			// B = (r'*z)/s;
			float B = r.dotProduct(z)/s;
			// y = -r + B*y;
			y.multiplyBy(B).subtractBy(r);
			// z = A*y;
			z = A.multiply(y);
			// s = y'*z;
			s = y.dotProduct(z);
			// t = (r'*y)/s;
			t = r.dotProduct(y) / s;
			// x = x + t*y;
			x.increaseBy(y.multiply(t));
			// fprintf('Residual: %f\n', norm(A*x-b));
			System.out.println("Residual (" + k + "): " + A.multiply(x).subtractBy(b).norm());
	    //end
		}

		// kill the executor service
		ExecutorServiceProvider.shutDownExecutor();

		return x;
	}
}
