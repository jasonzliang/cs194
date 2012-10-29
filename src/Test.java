import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import math.ConjugateGradientSolver;
import math.Matrix;
import math.Vector;


public class Test {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// A simple test
		// A needs to be a positive semi-definite matrix
		// ((zT)A(z)) > 0 for all positive values z
		float aVal[][] = { {2.0f, -1.0f, 0.0f}, 
		                   {-1.0f, 2.0f, -1.0f}, 
		                   {0.0f, -1.0f, 2.0f} };
		float bVal[] = { 0.0f, 0.0f, 4.0f };
		Matrix A = new Matrix(3, 3);
		for (int i=0; i<aVal.length; i++) {
			for (int j=0; j<aVal[0].length; j++) {
				A.setValue(i, j, aVal[i][j]);
			}
		}
		Vector b = new Vector(bVal);
		System.out.println(ConjugateGradientSolver.solve(A, b));

		Test.readMatrixFile("matrix.txt.mtx");
		Test.readVectorFile("vector.txt.mtx");
	}

	public static Matrix readMatrixFile(String fileName) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(fileName));
		String line;
		String temp[];
		Matrix m = null;
		while ((line = br.readLine()) != null) {
			if (line.startsWith("%")) {
				continue;
			}
			temp = line.split(" ");
			if (m == null) {
				// this should only happen for the first line of content, which
				// tells us the matrix size
				m = new Matrix(Integer.parseInt(temp[0]) + 1,
				               Integer.parseInt(temp[1]) + 1);
				continue;
			}
			// this is an entry
			m.setValue(Integer.parseInt(temp[0]),
		               Integer.parseInt(temp[1]),
                       Float.parseFloat(temp[2]));
		}
		return null;
	}

	public static Vector readVectorFile(String fileName) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(fileName));
		String line;
		String temp[];
		Vector v = null;
		while ((line = br.readLine()) != null) {
			if (line.startsWith("%")) {
				continue;
			}
			temp = line.split(" ");
			if (v == null) {
				// this should only happen for the first line of content, which
				// tells us the vector
				v = new Vector(Integer.parseInt(temp[1]) + 1);
				continue;
			}
			// this is an entry
			v.setValue(Integer.parseInt(temp[1]),
                       Float.parseFloat(temp[2]));
		}
		return v;
	}
}
