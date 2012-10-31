import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import math.ArrayVector;
import math.ConjugateGradientSolver;
import math.Matrix;
import math.Vector;


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
		Matrix A = new Matrix(3, 3);
		for (int i=0; i<aVal.length; i++) {
			for (int j=0; j<aVal[0].length; j++) {
				A.setValue(i, j, aVal[i][j]);
			}
		}
		Vector b = new ArrayVector(bVal);
		System.out.println(ConjugateGradientSolver.solve(A, b));
		*/

		Matrix A = Driver.readMatrixFile("matrix.txt.mtx");
		Vector b = Driver.readVectorFile("vector.txt.mtx");
		//printVectorToFile(b, "java_b.mtx");
		//printMatrixToFile(A, "java_A.mtx");
		Matrix P = Driver.readMatrixFile("P.mtx");
		long start = System.nanoTime();
		//Vector solution = ConjugateGradientSolver.solve(A, b);
		Vector solution = ConjugateGradientSolver.pcgSolve(A, b, P);
		double time = ((double) (System.nanoTime() - start))/1000000000;
		
		System.out.println(time);
		
		String fileName = "java_solution.txt.mtx";
		if (args.length > 0) {
			fileName = args[0] + ".txt.mtx";
		}
		printVectorToFile(solution, fileName);
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

	public static void printVectorToFile(Vector v, String fileName) throws IOException {
		int numEntries = 0;
		for (int i=0; i<v.getSize(); i++) {
			if (v.getValue(i) != 0.0f) {
				numEntries++;
			}
		}
		BufferedWriter out = new BufferedWriter(new FileWriter(fileName));
		out.write("%%MatrixMarket matrix coordinate real general");
		out.newLine();
		out.write("%");
		out.newLine();
		out.write("1 " + (v.getSize() - 1) + " " + numEntries);
		out.newLine();
		for (int i=0; i<v.getSize(); i++) {
			if (v.getValue(i) != 0.0f) {
				out.write("1 " + i + " " + v.getValue(i));
				out.newLine();
			}
		}
		out.close();
	}

	public static void printMatrixToFile(Matrix m, String fileName) throws IOException {
		int numEntries = 0;
		for (int i=0; i<m.getM(); i++) {
			numEntries += m.getNonZeroRowIndecies(i).size();
		}
		BufferedWriter out = new BufferedWriter(new FileWriter(fileName));
		out.write("%%MatrixMarket matrix coordinate real general");
		out.newLine();
		out.write("%");
		out.newLine();
		out.write((m.getM() - 1) + " " + (m.getN() - 1) + " " + numEntries);
		out.newLine();
		for (int i=0; i<m.getM(); i++) {
			for (int j : m.getNonZeroRowIndecies(i)) {
				if (m.getValue(i, j) != 0.0f) {
					out.write(i + " " + j + " " + m.getValue(i, j));
					out.newLine();
				}
			}
		}
		out.close();
	}
}
