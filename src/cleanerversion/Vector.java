package cleanerversion;

public interface Vector extends PrintableElementContainer {
	/**
	 * @return a deep clone of this vector
	 */
	public Vector clone();

	/**
	 * @return the size of this vector
	 */
	public int getSize();

	/**
	 * @param i the index of the element
	 * @return the element at i
	 */
	public float getValue(int i);

	/**
	 * @param i the index of the element
	 * @param value the value to set the element at index i to
	 */
	public void setValue(int i, float value);

	/**
	 * @param v2 the other vector
	 * @return the scalar dot product of v*v2
	 */
	public float dotProduct(Vector v2);

	/**
	 * @return the dot product of this vector with itself
	 */
	public float norm();

	/**
	 * @param v2 the vector to add
	 * @return a new vector that's the result of v + v2
	 */
	public Vector add(Vector v2);

	/**
	 * @param v2 the vector to subtract
	 * @return a new vector that's the result of v - v2
	 */
	public Vector subtract(Vector v2);

	/**
	 * Adds <code>v2</code> to <code>this</code> vector, mutating it.
	 * @param v2 the vector to add
	 * @return <code>this</code> vector after mutation
	 */
	public Vector increaseBy(Vector v2);

	/**
	 * Subtracts <code>v2</code> from <code>this</code> vector, mutating it.
	 * @param v2 the vector to subtract
	 * @return <code>this</code> vector after mutation
	 */
	public Vector subtractBy(Vector v2);

	/**
	 * @param s the value to multiply by
	 * @return a new vector that's the result of s*v
	 */
	public Vector multiply(float s);

	/**
	 * Multiplies <code>this</code> vector's elements by <code>s</code>, mutating it.
	 * @param s the element to multiply by
	 * @return <code>this</code> vector after mutation
	 */
	public Vector multiplyBy(float s);
}
