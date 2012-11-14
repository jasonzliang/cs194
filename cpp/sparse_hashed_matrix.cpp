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
    M = 0;
    N = 0;
  }

  SparseHashedMatrix<T>(int m, int n) {
    M = m;
    N = n;
    values = new map<int, T>[N];
  }

  SparseHashedMatrix<T>(const SparseHashedMatrix<T> &m2) {
    M = m2.getM();
    N = m2.getN();

    typename map<int, T>::iterator itr;
    for (int row=0; row<getN(); row++) {
      for (itr = m2.values[row].begin(); itr != m2.values[row].end(); ++itr) {
	setValue(itr->first, row, itr->second);
      }
    }
  }

  SparseHashedMatrix<T>(string fileName) {
    M = 0;
    N = 0;
    readFromMatrixMarketFile(fileName);
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
    for (int row=0; row<10; row++) {
      for (int column=0; column<10; column++) {
	cout << " " << getValue(column, row);
      }
      cout << endl;
    }
  }

  void readFromMatrixMarketFile(string fileName) {
    int row, col;
    T val; // really this has to be double or float
    
    string line;
    ifstream mmFile (fileName.c_str());
    
    if (mmFile.is_open()) {
      while (mmFile.good()) {
	getline(mmFile, line);
	
	if (line[0] == '%') {
	  continue;
	}

	std::stringstream lineStream(line);
	
	lineStream >> row >> col  >> val;
	
       
	if (M == 0 && N == 0) { // not allocated yet
	  N = row + 1;
	  M = col + 1;
	  values = new map<int, T>[N];
	  cout << "Matrix size: " << M << ", " << N << endl;

	  continue;
	}
	
	// Not too sure if I should swap row with col
	setValue(col, row, val);

	// TODO : remove. for checking only
	//cout << "Setting val: col " << col << ", row" << row << " val " << val << endl;
	//cout << getValue(col, row) << endl;
      }

      mmFile.close();
    }

  }


};
