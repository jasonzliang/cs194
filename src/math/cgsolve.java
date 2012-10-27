package math;

public class cgsolve {
	  public static Vector solve(Matrix A, Vector b) {
		  return solve(A, b, 1e-8);
	  }
	  public static Vector solve(Matrix A, Vector b, double tol) {
	    // x = cgsolve(A, b, tol)
	    Vector x = new Vector(b.getSize()); // x = zeros(size(b));
	    Vector p = new Vector(b.getSize()); // p = zeros(size(b));
	    Vector Ap = new Vector(b.getSize()); // Ap = zeros(size(b));
	    Vector r = b.clone(); // r = b;
	    double rtr = r.norm(); // rtr = r' * r;
	    double rtrold;
	    double beta = 0.0;
	    double alpha;
	    while (r.norm() > tol * b.norm()) { // while (norm(r) > tol * norm(b))
	      p = r.vectorAddition(p.scalarMultiplication(beta)); // p = r + beta * p;
	      Ap = p.matrixVectorMultiplication(A); // Ap = A * p;
	      // A = monitor.select(A); // representation selection, see Sec. 4
	      alpha = rtr / p.dotProduct(Ap); // alpha = rtr / (p' * Ap);
	      x = p.scalarMultiplication(alpha); // x = x + alpha * p;
	      r = Ap.scalarMultiplication(alpha); // r = r - alpha * Ap;
	      rtrold = rtr;
	      rtr = r.dotProduct(r); // rtr = r' * r;
	      beta = rtr / rtrold;
	      // iteration++; // benchmark purpose only, see Sec. 5
	      // if(iteration >= 10) break; // benchmark purpose only, see Sec. 5
	    }
	    return x;
	  } // end of method cgsolve
	} // end of class cgsolve