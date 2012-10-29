package math;

import java.util.Hashtable;
import java.util.Map;

public class MappedVector implements Vector {
	private Map<Integer, Float> vectorValues = null;
	private int size;

	/**
	 * Creates a zero vector of the given <code>size</code>
	 * @param size
	 */
	public MappedVector(int size) {
		vectorValues = new Hashtable<Integer, Float>();
		this.size = size;
	}

	/**
	 * Creates a vector with the given <code>values</code>.
	 * This creates a <strong>deep copy</strong>, which means the input
	 * <code>values</code> array will not be mutated.
	 * @param values
	 */
	public MappedVector(float values[]) {
		this(values.length);
		for (int i=0; i<values.length; i++) {
			vectorValues.put(i, values[i]);
		}
	}

	public MappedVector(MappedVector v) {
		Map<Integer, Float> values = v.vectorValues;
		vectorValues = new Hashtable<Integer, Float>(values.size() * 3 / 2);
		for (int k : values.keySet()) {
			vectorValues.put(k, values.get(k));
		}
		this.size = v.getSize();
	}

	@Override
	public MappedVector clone() {
		return new MappedVector(this);
	}

	@Override
	public int getSize() {
		return size;
	}

	@Override
	public float getValue(int index) {
		if (vectorValues.get(index) != null) {
			return vectorValues.get(index);
		} else {
			return 0.0f;
		}
	}

	@Override
	public void setValue(int index, float value) {
		vectorValues.put(index, value);
	}

	@Override
	public MappedVector matrixVectorMultiplication(Matrix m) {
		if (m.getN() != getSize()) {
			throw new IllegalArgumentException("The matrix's width (N) must "
					+ "be the same as this vector's size");
		}
		MappedVector result = new MappedVector(m.getM());
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
	public MappedVector vectorAddition(Vector v) {
		MappedVector result = new MappedVector(this);
		result.add(v);
		return result;
	}

	@Override
	public MappedVector vectorSubtraction(Vector v) {
		MappedVector result = new MappedVector(this);
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
	public Vector scalarMultiplication(float s) {
		MappedVector result = new MappedVector(this);
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
		for (int i=0; i<getSize(); i++) {
			if (addComma) {
				s.append(", ");
			}
			s.append(getValue(i));
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
