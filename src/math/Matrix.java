package math;

public class Matrix {
	float matrixValues[][] = null;

	public Matrix(int m, int n) {
		matrixValues = new float[m][n];
	}

	public Matrix(float values[][]) {
		matrixValues = new float[values.length][values[0].length];
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

	public float getValue(int i, int j) {
		return matrixValues[i][j];
	}

	public void setValue(int i, int j, float value) {
		matrixValues[i][j] = value;
	}

}
