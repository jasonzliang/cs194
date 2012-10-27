package math;

public class Matrix {
	double matrixValues[][] = null;

	public Matrix(int m, int n) {
		matrixValues = new double[m][n];
	}

	public Matrix(double values[][]) {
		matrixValues = new double[values.length][values[0].length];
		for (int i=0; i<values.length; i++) {
			for (int j=0; j<values[0].length; j++) {
				matrixValues[i][j] = values[i][j];
			}
		}
	}

	public int getM() {
		return matrixValues.length;
	}

	public int getN() {
		return matrixValues[0].length;
	}

	public double getValue(int i, int j) {
		return matrixValues[i][j];
	}

	public void setValue(int i, int j, double value) {
		matrixValues[i][j] = value;
	}

}
