package math;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Matrix {
	private Map<Integer, Float> matrixValues[] = null;
	private int m, n;

	public Matrix(int m, int n) {
		matrixValues = (Map<Integer, Float>[]) new Map[m];
		for (int i=0; i<m; i++) {
			matrixValues[i] = new HashMap<Integer, Float>();
		}
		this.m = m;
		this.n = n;
	}

	public int getM() {
		return m;
	}

	public int getN() {
		return n;
	}

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

	public void setValue(int i, int j, float value) {
		matrixValues[i].put(j, value);
	}

	@Override
	public String toString() {
		StringBuilder s = new StringBuilder();
		List<Integer> l = new ArrayList<Integer>();
		boolean addLineBreak = false;
		for (int i=0; i<matrixValues.length; i++) {
			l.addAll(matrixValues[i].keySet());
			Collections.sort(l);
			boolean addComma = false;
			if (addLineBreak) {
				s.append("\n");
			}
			for (int j : l) {
				if (addComma) {
					s.append(", ");
				}
				s.append("[" + i + ", " + j + "]: " + getValue(i, j));
				addComma = true;
			}
			addLineBreak = true;
			l.removeAll(matrixValues[i].keySet());
		}
		return s.toString();
	}
}
