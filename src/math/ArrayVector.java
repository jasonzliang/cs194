package math;

public class ArrayVector implements Vector {
	private float vectorValues[] = null;

	/**
	 * Creates a zero vector of the given <code>size</code>
	 * @param size
	 */
	public ArrayVector(int size) {
		vectorValues = new float[size + 1];
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
	}

	@Override
	public ArrayVector clone() {
		return new ArrayVector(vectorValues);
	}

	@Override
	public int getSize() {
		return vectorValues.length - 1;
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
		ArrayVector result = new ArrayVector(m.getM());
		// result[i] = sum_j(this[j] * m[i][j])
		float temp;
		for (int i=0; i<m.getM(); i++) {
			temp = 0.0f;
			for (int j : m.getNonZeroRowIndecies(i)) {
				temp += getValue(j) * m.getValue(i, j);
			}
			result.setValue(i, temp);
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
}
