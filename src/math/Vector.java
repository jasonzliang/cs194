package math;

public class Vector {
	private double vectorValues[] = null;

	/**
	 * Creates a zero vector of the given <code>size</code>
	 * @param size
	 */
	public Vector(int size) {
		vectorValues = new double[size];
	}

	/**
	 * Creates a vector with the given <code>values</code>.
	 * This creates a <strong>deep copy</strong>, which means the input
	 * <code>values</code> array will not be mutated.
	 * @param values
	 */
	public Vector(double values[]) {
		this.vectorValues = new double[values.length];
		for (int i=0; i<values.length; i++) {
			this.vectorValues[i] = values[i];
		}
	}

	public Vector clone() {
		return new Vector(vectorValues);
	}

	public int getSize() {
		return vectorValues.length;
	}

	public double getValue(int index) {
		return vectorValues[index];
	}

	public void setValue(int index, double value) {
		vectorValues[index] = value;
	}

	/**
	 * @param m the matrix from <code>m</code>*(this vector)
	 * @return a new vector with the result of <code>m</code>*(this vector)
	 */
	public Vector matrixVectorMultiplication(Matrix m) {
		if (m.getN() != getSize()) {
			throw new IllegalArgumentException("The matrix's width (N) must "
					+ "be the same as this vector's size");
		}
		Vector result = new Vector(m.getM());
		// result[i] = sum_j(this[j] * m[i][j])
		double temp;
		for (int i=0; i<m.getM(); i++) {
			temp = 0.0;
			for (int j=0; j<getSize(); j++) {
				temp += getValue(j) * m.getValue(i, j);
			}
			result.setValue(i, temp);
		}
		return result;
	}

	/**
	 * @param v
	 * @return the scalar dot product
	 */
	public double dotProduct(Vector v) {
		verifyVectorSizesMatch(this, v);
		double dotProduct = 0.0;
		for (int i=0; i<getSize(); i++) {
			dotProduct += this.getValue(i) * v.getValue(i);
		}
		return dotProduct;
	}

	/**
	 * @return the sum of the squares of the elements of this vector
	 */
	public double norm() {
		return dotProduct(this);
	}

	/**
	 * @param v the vector to add
	 * @return a new vector that is the result of (this vector)+<code>v</code>
	 */
	public Vector vectorAddition(Vector v) {
		Vector result = new Vector(vectorValues);
		result.add(v);
		return result;
	}

	/**
	 * @param v the vector to add
	 * @return a new vector that is the result of (this vector)-<code>v</code>
	 */
	public Vector vectorSubtraction(Vector v) {
		Vector result = new Vector(vectorValues);
		result.subtract(v);
		return result;
	}

	/**
	 * Mutates this vector by adding <code>v</code> to it
	 * @param v the vector to add
	 */
	public void add(Vector v) {
		verifyVectorSizesMatch(this, v);
		for (int i=0; i<getSize(); i++) {
			setValue(i, this.getValue(i) + v.getValue(i));
		}
	}

	/**
	 * Mutates this vector by subtracting <code>v</code> from it.<br>
	 * This means mathematically, we're doing:<br>
	 * (this vector) = (this vector) - v
	 * @param v the vector to subtract
	 */
	public void subtract(Vector v) {
		verifyVectorSizesMatch(this, v);
		for (int i=0; i<getSize(); i++) {
			setValue(i, this.getValue(i) - v.getValue(i));
		}
	}

	/**
	 * @param s the scalar value to multiply by
	 * @return a new vector that is s * (this vector)
	 */
	public Vector scalarMultiplication(double s) {
		Vector result = new Vector(vectorValues);
		result.multiply(s);
		return result;
	}

	/**
	 * Mutates this vector by multiply each element by <code>s</code>
	 * @param s the scalar to multiply by
	 */
	public void multiply(double s) {
		for (int i=0; i<getSize(); i++) {
			setValue(i, s * getValue(i));
		}
	}

	public String toString() {
		StringBuilder s = new StringBuilder();
		s.append("[");
		boolean addComma = false;
		for (double val : vectorValues) {
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
