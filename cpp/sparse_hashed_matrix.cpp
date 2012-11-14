#include "matrix.h"
#include "vector.h"
#include "matrix_market_handler.h"
#include <iostream>
#include <map>

using namespace std;

template <typename T>
class SparseHashedMatrix : public Matrix<T> {
  private:
  map<int, T> *values;
  int M;
  int N;
  SparseHashedMatrix<T>() {}

  public:
  SparseHashedMatrix<T>(int m, int n) {
    M = m;
    N = n;
    values = new map<int, T>[N];
  }

  SparseHashedMatrix<T>(const SparseHashedMatrix<T> &m2) {
    SparseHashedMatrix newSHM = SparseHashedMatrix(m2.getM(), m2.getN());
    // TODO: finish this!
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
    return values[row][column];
  }

  void setValue(int column, int row, T value) {
    values[row][column] = value;
  }

  void multiply(Vector<T> &v, Vector<T> &result) const {
    // TODO: implement
  }

  void printToMatrixMarketFile(string fileName) {
    // TODO: implement
  }

};
