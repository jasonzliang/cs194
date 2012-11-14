#include "vector.h"
#include "printable_element_container.h"
#include <iostream>
#include <malloc.h>

using namespace std;

template <typename T>
class ArrayVector : Vector<T> {
  private:
  T *values;
  int size;

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
    free(values);
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
      temp += getValue(i) * v2.getValue(i);
    }
    return temp;
  }

  void increaseBy(Vector<T> &v2) {
    for (int i=0; i<getSize(); i++) {
      values[i] += v2.getValue(i);
    }
  }

  void reduceBy(Vector<T> &v2) {
    for (int i=0; i<getSize(); i++) {
      values[i] -=  v2.getValue(i);
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
      result.setValue(i, values[i] + v2.getValue(i));
    }
    return result;
  }

  const ArrayVector<T> operator-(const Vector<T> &v2) const {
    ArrayVector<T> result(getSize());
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] - v2.getValue(i));
    }
    return result;
  }

  const Vector<T> operator*(const T s) const {
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
      cout << i << ": " << values[i] << endl;
    }
  }
};
