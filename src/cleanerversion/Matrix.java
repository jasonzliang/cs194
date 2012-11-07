package cleanerversion;

public interface Matrix extends PrintableElementContainer {
	/**
	 * @return a deep clone of this matrix
	 */
	public Matrix clone();

	/**
	 * @return the number of horizontal elements
	 */
	public int getM();

	/**
	 * @return the number of vertical elements
	 */
	public int getN();

	/**
	 * @param column the horizontal index
	 * @param row the vertical index
	 * @return the element at (row, column)
	 */
	public float getValue(int column, int row);

	/**
	 * @param column the horizontal index
	 * @param row the vertical index
	 * @param value the value to set the element to
	 */
	public void setValue(int column, int row, float value);

	/**
	 * @param v the vector to multiply by
	 * @return a new vector that's the result of M*v
	 */
	public Vector multiply(Vector v);
}
