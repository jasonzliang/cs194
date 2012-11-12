package math;

public interface Vector {
	public Vector clone();

	public int getSize();

	public float getValue(int index);

	public void setValue(int index, float value);

	/**
	 * @param m the matrix from <code>m</code>*(this vector)
	 * @return a new vector with the result of <code>m</code>*(this vector)
	 */
	public Vector matrixVectorMultiplication(Matrix m);

	/**
	 * @param v
	 * @return the scalar dot product
	 */
	public float dotProduct(Vector v);

	/**
	 * @return the sum of the squares of the elements of this vector
	 */
	public float norm();

	/**
	 * @param v the vector to add
	 * @return a new vector that is the result of (this vector)+<code>v</code>
	 */
	public Vector vectorAddition(Vector v);

	/**
	 * @param v the vector to add
	 * @return a new vector that is the result of (this vector)-<code>v</code>
	 */
	public Vector vectorSubtraction(Vector v);

	/**
	 * Mutates this vector by adding <code>v</code> to it
	 * @param v the vector to add
	 */
	public void add(Vector v);

	/**
	 * Mutates this vector by subtracting <code>v</code> from it.<br>
	 * This means mathematically, we're doing:<br>
	 * (this vector) = (this vector) - v
	 * @param v the vector to subtract
	 */
	public void subtract(Vector v);

	/**
	 * @param s the scalar value to multiply by
	 * @return a new vector that is s * (this vector)
	 */
	public Vector scalarMultiplication(float s);

	/**
	 * Mutates this vector by multiply each element by <code>s</code>
	 * @param s the scalar to multiply by
	 */
	public void multiply(float s);
}
