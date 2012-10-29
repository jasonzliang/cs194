package math;

import java.util.Hashtable;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.Lock;

public class Matrix {
	private Map<Integer, Float> matrixValues[] = null;
	private int m, n;
	private Lock lock[];

	public Matrix(int m, int n) {
		matrixValues = (Hashtable<Integer, Float>[]) new Hashtable[m];
		for (int i=0; i<m; i++) {
			matrixValues[i] = new Hashtable<Integer, Float>();
		}
		this.m = m;
		this.n = n;
		lock = new Lock[m];
	}

	public int getM() {
		return m;
	}

	public int getN() {
		return n;
	}

	// needs to be locked on i!
	public float getValue(int i, int j) {
		if (matrixValues[i].get(j) != null) {
			return matrixValues[i].get(j);
		} else {
			return 0.0f;
		}
	}

	public Set<Integer> getNonZeroRowIndecies(int row) {
		return matrixValues[row].keySet();
	}

	// needs to be locked on i!
	public void setValue(int i, int j, float value) {
		matrixValues[i].put(j, value);
	}

}
