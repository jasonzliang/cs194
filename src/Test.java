import math.ConjugateGradientSolver;
import math.Matrix;
import math.Vector;


public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// A simple test
		// A needs to be a positive semi-definite matrix
		// ((zT)A(z)) > 0 for all positive values z
		double aVal[][] = { {2, -1, 0}, 
				            {-1, 2, -1}, 
				            {0, -1, 2} };
		double bVal[] = { 0, 0, 4 };
		Matrix A = new Matrix(aVal);
		Vector b = new Vector(bVal);
		System.out.println(ConjugateGradientSolver.solve(A, b));
	}

}
