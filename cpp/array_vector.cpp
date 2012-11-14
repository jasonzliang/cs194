#include "vector.h"
#include "matrix_market_handler.h"
#include <iostream>
#include <malloc.h>
#include <fstream>

using namespace std;

template <typename T>
class ArrayVector : public Vector<T> {
  private:
  T *values;
  int size;

  public:
  ArrayVector<T>() {
    this(0);
  }

  ArrayVector<T>(int s) {
    size = s;
    values = new T[size];
  }

  ArrayVector<T>(const ArrayVector<T> &v2) {
    ArrayVector newAV = ArrayVector(v2.getSize());
    for (int i=0; i<v2.getSize(); i++) {
      newAV.values[i] = v2.values[i];
    }
  }

  ~ArrayVector<T>() {
    delete[] values;
  }

  void resizeVector(int newSize) {
    delete[] values;
    size = newSize;
    values = new T[newSize];
  }

  int getSize() const {
    return size;
  }

  T getValue(int i) const {
    return values[i];
  }

  void setValue(int i, T value) {
    values[i] = value;
  }

  T dotProduct(Vector<T> &v2) const {
    T temp;
    for (int i=0; i<getSize(); i++) {
      temp += values[i] * v2[i];
    }
    return temp;
  }

  void increaseBy(Vector<T> &v2) {
    for (int i=0; i<getSize(); i++) {
      values[i] += v2[i];
    }
  }

  void reduceBy(Vector<T> &v2) {
    for (int i=0; i<getSize(); i++) {
      values[i] -=  v2[i];
    }
  }

  void scaleBy(T s) {
    for (int i=0; i<getSize(); i++) {
      values[i] *= s;
    }
  }

  void add(const Vector<T> &v2, Vector<T> &result) const {
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] + v2[i]);
    }
  }

  void subtract(const Vector<T> &v2, Vector<T> &result) const {
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] - v2[i]);
    }
  }

  void multiply(const T s, Vector<T> &result) const {
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] * s);
    }
  }

  void printToMatrixMarketFile(string fileName) {
    // TODO: implement properly
    ofstream mmFile (fileName.c_str());

    if (mmFile.is_open()) {
      // this is just for testing...
      for (int i=0; i<getSize(); i++) {
	if (i > 0) {
	  mmFile << ", ";
	}
	mmFile << values[i];
      }
      mmFile << endl;
      
      mmFile.close();
    } else {
      cout << "Problem writing matrix market file" << endl;
    }
  }

  void print() {
    for (int i=0; i<getSize(); i++) {
      if (i > 0) {
	cout << ", ";
      }
      cout << values[i];
    }
    cout << endl;
  }

  void readFromMatrixMarketFile(string fileName) {
    string line;
    ifstream mmFile (fileName.c_str());
    
    if (mmFile.is_open()) {
      while (mmFile.good()) {
	getline(mmFile, line);
	cout << line << endl;	
      }
    }
  }

};
