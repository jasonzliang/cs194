#include "matrix.h"
#include "vector.h"
#include "matrix_market_handler.h"
#include <iostream>
#include <map>
#include <iterator>

using namespace std;

template <typename T>
class SparseHashedMatrix : public Matrix<T> {
  private:
  map<int, T> *values;
  int M;
  int N;

  public:
  SparseHashedMatrix<T>() {
    this(0, 0);
  }

  SparseHashedMatrix<T>(int m, int n) {
    M = m;
    N = n;
    values = new map<int, T>[N];
  }

  SparseHashedMatrix<T>(const SparseHashedMatrix<T> &m2) {
    SparseHashedMatrix newSHM = SparseHashedMatrix(m2.getM(), m2.getN());
    typename map<int, T>::iterator itr;
    for (int row=0; row<getN(); row++) {
      for (itr = values[row].begin(); itr != values[row].end(); ++itr) {
	m2.setValue(itr->first, row, itr->second);
      }
    }
  }

  ~SparseHashedMatrix<T>() {
    delete[] values;
  }

  int getM() const {
    return M;
  }

  int getN() const {
    return N;
  }

  T getValue(int column, int row) const {
    if (values[row].find(column) == values[row].end()) {
      return 0;
    } else {
      return values[row].find(column)->second;
    }
  }

  void setValue(int column, int row, T value) {
    values[row][column] = value;
  }

  void multiply(Vector<T> &v, Vector<T> &result) const {
    typename map<int, T>::iterator itr;
    for (int row=0; row<getN(); row++) {
      T temp = 0;
      for (itr = values[row].begin(); itr != values[row].end(); ++itr) {
	temp += v.getValue(itr->first) * itr->second;
      }
      result.setValue(row, temp);
    }
  }

  void printToMatrixMarketFile(string fileName) {
    // TODO: implement
  }

  // seriously don't use this except when testing
  // with large matrices, things will go kaplooy
  void print() {
    for (int row=0; row<getN(); row++) {
      for (int column=0; column<getM(); column++) {
	cout << " " << getValue(column, row);
      }
      cout << endl;
    }
  }

};
