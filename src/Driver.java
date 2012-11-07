import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import cleanerversion.ArrayVector;
import cleanerversion.ConjugateGradientSolver;
import cleanerversion.ExecutorServiceProvider;
import cleanerversion.Matrix;
import cleanerversion.SparseHashedMatrix;
import cleanerversion.Vector;


public class Driver {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// A simple test
		// A needs to be a positive semi-definite matrix
		// ((zT)A(z)) > 0 for all positive values z
		/*
		float aVal[][] = { {2.0f, -1.0f, 0.0f}, 
		                   {-1.0f, 2.0f, -1.0f}, 
		                   {0.0f, -1.0f, 2.0f} };
		float bVal[] = { 0.0f, 0.0f, 4.0f };
		Matrix A = new SparseHashedMatrix(3, 3);
		for (int i=0; i<aVal.length; i++) {
			for (int j=0; j<aVal[0].length; j++) {
				A.setValue(i, j, aVal[i][j]);
			}
		}
		Vector b = new ArrayVector(3);
		for (int i=0; i<bVal.length; i++) {
			b.setValue(i, bVal[i]);
		}
		System.out.println(ConjugateGradientSolver.solve(A, b));
		*/
		if (args.length > 0) {
			int threads = Integer.parseInt(args[0]);
			if (threads > 0) {
				ExecutorServiceProvider.setNumConcurrentThreads(threads);
			}
		}

		Matrix A = Driver.readMatrixFile("matrix.txt.mtx");
		Vector b = Driver.readVectorFile("vector.txt.mtx");
		long start = System.nanoTime();
		Vector solution = ConjugateGradientSolver.solve(A, b);
		double time = ((double) (System.nanoTime() - start))/1000000000;
		
		System.out.println(time);
		System.out.println("Num threads: " + ExecutorServiceProvider.getNumConcurrentThreads());
		
		solution.printToMatrixMarketFile("java_solution", true);
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
				m = new SparseHashedMatrix(Integer.parseInt(temp[0]) + 1,
				                           Integer.parseInt(temp[1]) + 1);
				continue;
			}
			// this is an entry
			m.setValue(Integer.parseInt(temp[0]),
			           Integer.parseInt(temp[1]),
			           Float.parseFloat(temp[2]));
		}
		br.close();
		return m;
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
				v = new ArrayVector(Integer.parseInt(temp[1]) + 1);
				continue;
			}
			// this is an entry
			v.setValue(Integer.parseInt(temp[1]),
			           Float.parseFloat(temp[2]));
		}
		br.close();
		return v;
	}
}
