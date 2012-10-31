package math;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ArrayVector implements Vector {
	private float vectorValues[] = null;
	private MatrixVectorMultiplier multiplier[] = null;
	ExecutorService executor = null;

	/**
	 * Creates a zero vector of the given <code>size</code>
	 * @param size
	 */
	public ArrayVector(int size) {
		vectorValues = new float[size];
		prepareParallel();
	}

	/**
	 * Creates a vector with the given <code>values</code>.
	 * This creates a <strong>deep copy</strong>, which means the input
	 * <code>values</code> array will not be mutated.
	 * @param values
	 */
	public ArrayVector(float values[]) {
		this.vectorValues = new float[values.length];
		for (int i=0; i<values.length; i++) {
			this.vectorValues[i] = values[i];
		}
		prepareParallel();
	}

	private void prepareParallel() {
		int numProcessors = Runtime.getRuntime().availableProcessors() * 2;
		multiplier = new MatrixVectorMultiplier[numProcessors];
		executor = Executors.newFixedThreadPool(numProcessors);
	}

	@Override
	public ArrayVector clone() {
		return new ArrayVector(vectorValues);
	}

	@Override
	public int getSize() {
		return vectorValues.length;
	}

	@Override
	public float getValue(int index) {
		return vectorValues[index];
	}

	@Override
	public void setValue(int index, float value) {
		vectorValues[index] = value;
	}

	@Override
	public ArrayVector matrixVectorMultiplication(Matrix m) {
		if (m.getN() != getSize()) {
			throw new IllegalArgumentException("The matrix's width (N) must "
					+ "be the same as this vector's size");
		}
		ArrayVector result = new ArrayVector(getSize());
		// result[i] = sum_j(this[j] * m[i][j])
		boolean multithread = false; // doesn't exit yet ;-(
		if (!multithread) {
			float temp;
			for (int i=0; i<m.getM(); i++) {
				temp = 0.0f;
				for (int j : m.getNonZeroRowIndecies(i)) {
					temp += getValue(j) * m.getValue(i, j);
				}
				result.setValue(i, temp);
			}
		} else {
			int numThreads = multiplier.length;
			CountDownLatch mainLatch = new CountDownLatch(numThreads);
			int perThread = m.getM() / numThreads;
			for (int i=0; i<numThreads; i++) {
				int startIndex = i * perThread;
				int endIndex = startIndex + perThread - 1;
				if (i+1 == numThreads) {
					endIndex = m.getM() - 1;
				}
				multiplier[i] = new MatrixVectorMultiplier(m, result, startIndex, endIndex);
				multiplier[i].setLatch(mainLatch);
				executor.execute(multiplier[i]);
			}
			try {
				mainLatch.await();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		return result;
	}

	@Override
	public float dotProduct(Vector v) {
		verifyVectorSizesMatch(this, v);
		float dotProduct = 0.0f;
		for (int i=0; i<getSize(); i++) {
			dotProduct += this.getValue(i) * v.getValue(i);
		}
		return dotProduct;
	}

	@Override
	public float norm() {
		return dotProduct(this);
	}

	@Override
	public ArrayVector vectorAddition(Vector v) {
		ArrayVector result = new ArrayVector(vectorValues);
		result.add(v);
		return result;
	}

	@Override
	public ArrayVector vectorSubtraction(Vector v) {
		ArrayVector result = new ArrayVector(vectorValues);
		result.subtract(v);
		return result;
	}

	@Override
	public void add(Vector v) {
		verifyVectorSizesMatch(this, v);
		for (int i=0; i<getSize(); i++) {
			setValue(i, this.getValue(i) + v.getValue(i));
		}
	}

	@Override
	public void subtract(Vector v) {
		verifyVectorSizesMatch(this, v);
		for (int i=0; i<getSize(); i++) {
			setValue(i, this.getValue(i) - v.getValue(i));
		}
	}

	@Override
	public ArrayVector scalarMultiplication(float s) {
		ArrayVector result = new ArrayVector(vectorValues);
		result.multiply(s);
		return result;
	}

	@Override
	public void multiply(float s) {
		for (int i=0; i<getSize(); i++) {
			setValue(i, s * getValue(i));
		}
	}

	@Override
	public String toString() {
		StringBuilder s = new StringBuilder();
		s.append("[");
		boolean addComma = false;
		for (float val : vectorValues) {
			if (addComma) {
				s.append(", ");
			}
			s.append(val);
			addComma = true;
		}
		s.append("]");
		return s.toString();
	}

	private void verifyVectorSizesMatch(Vector v1, Vector v2) {
		if (v1.getSize() != v2.getSize()) {
			throw new IllegalArgumentException("The vectors must be of the same size");
		}
	}

	private class MatrixVectorMultiplier implements Runnable {
		Matrix m = null;
		Vector result = null;
		int startIndex = 0;
		int endIndex = 0;
		CountDownLatch cdl = null;

		public MatrixVectorMultiplier(Matrix m, Vector result, int startIndex, int endIndex) {
			this.m = m;
			this.result = result;
			this.startIndex = startIndex;
			this.endIndex = endIndex;
		}

		public void setLatch(CountDownLatch cdl) {
			this.cdl = cdl;
		}

		@Override
		public void run() {
			float temp;
			for (int i=startIndex; i<=endIndex; i++) {
				temp = 0.0f;
				for (int j : m.getNonZeroRowIndecies(i)) {
					temp += getValue(j) * m.getValue(i, j);
				}
				result.setValue(i, temp);
			}
			cdl.countDown();
		}
	}
}
