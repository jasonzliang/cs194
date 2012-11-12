package cleanerversion;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class ArrayVector implements Vector {
	float values[] = null;

	public ArrayVector(int size) {
		values = new float[size];
	}

	@Override
	public Vector clone() {
		Vector clone = new ArrayVector(getSize());
		for (int i=0; i<getSize(); i++) {
			clone.setValue(i, getValue(i));
		}
		return null;
	}

	@Override
	public int getSize() {
		return values.length;
	}

	@Override
	public float getValue(int i) {
		return values[i];
	}

	@Override
	public void setValue(int i, float value) {
		values[i] = value;
	}

	@Override
	public float dotProduct(Vector v2) {
		verifyVectorSize(v2);

		float result = 0.0f;
		for (int i=0; i<getSize(); i++) {
			result += this.getValue(i) * v2.getValue(i);
		}

		return result;
	}

	@Override
	public float norm() {
		return dotProduct(this);
	}

	@Override
	public Vector add(Vector v2) {
		verifyVectorSize(v2);

		Vector result = new ArrayVector(getSize());
		for (int i=0; i<getSize(); i++) {
			result.setValue(i, this.getValue(i) + v2.getValue(i));
		}

		return result;
	}

	@Override
	public Vector subtract(Vector v2) {
		verifyVectorSize(v2);

		Vector result = new ArrayVector(getSize());
		for (int i=0; i<getSize(); i++) {
			result.setValue(i, this.getValue(i) - v2.getValue(i));
		}

		return result;
	}

	@Override
	public Vector increaseBy(Vector v2) {
		verifyVectorSize(v2);

		for (int i=0; i<getSize(); i++) {
			values[i] += v2.getValue(i);
		}

		return this;
	}

	@Override
	public Vector subtractBy(Vector v2) {
		verifyVectorSize(v2);

		for (int i=0; i<getSize(); i++) {
			values[i] -= v2.getValue(i);
		}

		return this;
	}

	@Override
	public Vector multiply(float s) {

		Vector result = new ArrayVector(getSize());
		for (int i=0; i<getSize(); i++) {
			result.setValue(i, this.getValue(i) * s);
		}

		return result;
	}

	@Override
	public Vector multiplyBy(float s) {

		for (int i=0; i<getSize(); i++) {
			values[i] *= s;
		}

		return this;
	}

	@Override
	public void printToMatrixMarketFile(String fileName) throws IOException {
		printToMatrixMarketFile(fileName, false);
	}

	@Override
	public void printToMatrixMarketFile(String fileName, boolean decrementCount) throws IOException {
		int numEntries = 0;
		for (int i=0; i<this.getSize(); i++) {
			if (this.getValue(i) != 0.0f) {
				numEntries++;
			}
		}
		int count = this.getSize();
		if (decrementCount) { count -= 1; }

		BufferedWriter out = new BufferedWriter(new FileWriter(fileName + ".mtx"));
		out.write("%%MatrixMarket matrix coordinate real general");
		out.newLine();
		out.write("%");
		out.newLine();
		out.write("1 " + count + " " + numEntries);
		out.newLine();
		for (int i=0; i<this.getSize(); i++) {
			if (values[i] != 0.0f) {
				out.write("1 " + i + " " + values[i]);
				out.newLine();
			}
		}
		out.close();
	}

	public String toString() {
		StringBuilder s = new StringBuilder();
		s.append("[");
		if (getSize() > 6) {
			for (int i=0; i<3; i++) {
				if (i > 0) {
					s.append(", ");
				}
				s.append(values[i]);
			}
			s.append(" ... ");
			for (int i=getSize()-3; i<getSize(); i++) {
				if (i > getSize() - 3) {
					s.append(", ");
				}
				s.append(values[i]);
			}
		} else {
			for (int i=0; i<values.length; i++) {
				if (i > 0) {
					s.append(", ");
				}
				s.append(values[i]);
			}
		}
		s.append("]");
		return s.toString();
	}

	private void verifyVectorSize(Vector v2) {
		if (this.getSize() != v2.getSize()) {
			throw new IllegalArgumentException("The vectors must be of the " +
					"same size");
		}
	}
}
