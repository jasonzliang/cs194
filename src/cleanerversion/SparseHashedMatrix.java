package cleanerversion;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class SparseHashedMatrix implements Matrix {
	private List<Map<Integer, Float>> values = null;
	private int N, M;

	public SparseHashedMatrix(int n, int m) {
		N = n;
		M = m;

		values = new ArrayList<Map<Integer, Float>>(N);
		for (int i=0; i<N; i++) {
			values.add(i, new HashMap<Integer, Float>());
		}
	}

	@Override
	public Matrix clone() {
		Matrix clone = new SparseHashedMatrix(N, M);

		for (int row=0; row<N; row++) {
			for (int column : getKeysOfNonZeroElementsForRow(row)) {
				clone.setValue(column, row, getValue(row, column));
			}
		}
		
		return clone;
	}

	private Collection<Integer> getKeysOfNonZeroElementsForRow(int row) {
		return values.get(row).keySet();
	}

	@Override
	public int getM() {
		return M;
	}

	@Override
	public int getN() {
		return N;
	}

	@Override
	public float getValue(int column, int row) {
		return values.get(row).get(column);
	}

	@Override
	public void setValue(int column, int row, float value) {
		values.get(row).put(column, value);
	}

	@Override
	public Vector multiply(Vector v) {
		if (v.getSize() != this.getM()) {
			throw new IllegalArgumentException("The vector must be of the " +
					"same length as the horizontal length of this matrix");
		}

		Vector result = new ArrayVector(v.getSize());

		// TODO: this should be threaded
		for (int row=0; row<N; row++) {
			float temp = 0.0f;
			for (int column : getKeysOfNonZeroElementsForRow(row)) {
				temp += this.getValue(column, row) * v.getValue(column);
			}
			result.setValue(row, temp);
		}

		return result;
	}

	@Override
	public void printToMatrixMarketFile(String fileName) throws IOException {
		printToMatrixMarketFile(fileName, false);
	}

	@Override
	public void printToMatrixMarketFile(String fileName, boolean decrementCount) throws IOException {
		int numEntries = 0;
		for (int i=0; i<this.getM(); i++) {
			numEntries += this.getKeysOfNonZeroElementsForRow(i).size();
		}
		int mCount = this.getM();
		int nCount = this.getN();
		if (decrementCount) {
			mCount -= 1;
			nCount -= 1;
		}

		BufferedWriter out = new BufferedWriter(new FileWriter(fileName + ".mtx"));
		out.write("%%MatrixMarket matrix coordinate real general");
		out.newLine();
		out.write("%");
		out.newLine();
		out.write(mCount + " " + nCount + " " + numEntries);
		out.newLine();
		for (int row=0; row<this.getN(); row++) {
			for (int column : this.getKeysOfNonZeroElementsForRow(row)) {
				if (this.getValue(column, row) != 0.0f) {
					out.write(row + " " + column + " " + this.getValue(column, row));
					out.newLine();
				}
			}
		}
		out.close();
	}
}
