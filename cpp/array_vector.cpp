#include "vector.h"
#include "printable_element_container.h"
#include <iostream>
#include <malloc.h>

using namespace std;

template <typename T>
class ArrayVector : public Vector<T> {
  private:
  T *values;
  int size;
  ArrayVector<T>() {}

  public:
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
    cout << "freedom!  ";
    printToMatrixMarketFile("blah");
    delete[] values;
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

  const ArrayVector<T> operator+(const Vector<T> &v2) const {
    ArrayVector<T> result(getSize());
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] + v2[i]);
    }
    return result;
  }

  const ArrayVector<T> operator-(const Vector<T> &v2) const {
    ArrayVector<T> result(getSize());
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] - v2[i]);
    }
    return result;
  }

  const ArrayVector<T> operator*(const T s) const {
    ArrayVector<T> result(getSize());
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] * s);
    }
    return result;
  }

  void printToMatrixMarketFile(string fileName) {
    // TODO: implement properly

    // this is just for testing...
    for (int i=0; i<getSize(); i++) {
      if (i > 0) {
	cout << ", ";
      }
      cout << values[i];
    }
    cout << endl;
  }

};
