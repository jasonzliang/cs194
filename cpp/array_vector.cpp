#include "vector.h"
#include "matrix_market_handler.h"
#include <iostream>
#include <malloc.h>


using namespace std;

template <typename T>
class ArrayVector : public Vector<T> {
  private:
  T *values;
  unsigned int size;
  void init(unsigned int s) {
    size = s;
    values = new T[size];
  }

  public:
  ArrayVector<T>() {
    init(0);
  }

  ArrayVector<T>(unsigned int s) {
    init(s);
  }

  ArrayVector<T>(string fileName) {
    init(0);
    readFromMatrixMarketFile(fileName);
  }

  ArrayVector<T>(const ArrayVector<T> &v2) {
    init(v2.getSize());

    for (unsigned int i=0; i<v2.getSize(); i++) {
      values[i] = v2.values[i];
    }
  }

  ~ArrayVector<T>() {
    delete[] values;
  }

  void resizeVector(unsigned int newSize) {
    delete[] values;
    init(newSize);
  }

  unsigned int getSize() const {
    return size;
  }

  T getValue(unsigned int i) const {
    return values[i];
  }

  void setValue(unsigned int i, T value) {
    values[i] = value;
  }

  T dotProduct(const Vector<T> &v2) const {
    T temp = 0.0;
    for (unsigned int i=0; i<getSize(); i++) {
      temp += values[i] * v2[i];
    }
    return temp;
  }

  void increaseBy(Vector<T> &v2) {
    for (unsigned int i=0; i<getSize(); i++) {
      values[i] += v2[i];
    }
  }

  void reduceBy(Vector<T> &v2) {
    for (unsigned int i=0; i<getSize(); i++) {
      values[i] -=  v2[i];
    }
  }

  void scaleBy(T s) {
    for (unsigned int i=0; i<getSize(); i++) {
      values[i] *= s;
    }
  }

  void add(const Vector<T> &v2, Vector<T> &result) const {
    for (unsigned int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] + v2[i]);
    }
  }

  void subtract(const Vector<T> &v2, Vector<T> &result) const {
    for (unsigned int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] - v2[i]);
    }
  }

  void multiply(const T s, Vector<T> &result) const {
    for (unsigned int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] * s);
    }
  }

  void printToMatrixMarketFile(string fileName) {
    // TODO: implement properly
    unsigned int numEntries = 0;
    
    for (unsigned int i = 0; i < getSize(); i++) {
      if (getValue(i) != 0.0) {
	numEntries++;
      }
    }

    ofstream mmFile (fileName.c_str());

    if (mmFile.is_open()) {
      
      mmFile << "%%MatrixMarket matrix coordinate real general" << endl;
      mmFile << "%" << endl;
      mmFile << "1" << " " << getSize() << " " <<numEntries << endl;
      
      for (unsigned int i=0; i<getSize(); i++) {
	if (getValue(i) != 0) {
	  mmFile << "1" << " " << i << " " << getValue(i) << endl;
	}
      }
      
      mmFile.close();
    } else {
      cout << "Problem writing matrix market file" << endl;
    }
  }

  void print() {
    for (unsigned int i=0; i<getSize(); i++) {
      if (i > 0) {
	cout << ", ";
      }
      cout << values[i];
    }
    cout << endl;
  }

  void readFromMatrixMarketFile(string fileName) {
    unsigned int row, col;
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
	
	if (size == 0) { // not allocated yet
	  size = col + 1;
	  cout << "Vector size: " << size << endl;
	  values = new T[size];	
	  continue;
	}
	
	setValue(col, val);
      }

      mmFile.close();
    }
  }

};
