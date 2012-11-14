#include "vector.h"
#include "printable_element_container.h"
#include <malloc.h>

template <typename T>
class ArrayVector : Vector<T> {
  private:
  T *values;
  int size;

  public:
  ArrayVector<T>(int s) {
    size = s;
    values = malloc(size * sizeof(T));
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

  const int getSize() {
    return size;
  }

  const T getValue(int i) {
    return values[i];
  }

  void setValue(int i, T value) {
    values[i] = value;
  }

  const T dotProduct(Vector<T> &v2) {
    T temp;
    for (int i=0; i<getSize(); i++) {
      temp += getValue(i) * v2.getValue(i);
    }
    return temp;
  }

  Vector<T> increaseBy(Vector<T> &v2) {
    for (int i=0; i<getSize(); i++) {
      values[i] +=  v2.getValue(i);
    }
    return this;
  }

  Vector<T> reduceBy(Vector<T> &v2) {
    for (int i=0; i<getSize(); i++) {
      values[i] -=  v2.getValue(i);
    }
    return this;
  }

  Vector<T> scaleBy(T s) {
    for (int i=0; i<getSize(); i++) {
      values[i] *= s;
    }
    return this;
  }

  Vector<T> & operator+(const Vector<T> &v2) {
    ArrayVector<T> result(this.getSize());
    for (int i=0; i<getSize(); i++) {
      result.setValue(i, values[i] + v2.getValue(i));
    }
    return result;
  }

  Vector<T> & operator-(const Vector<T> &v2) {
    // TODO: fix
    return this;
  }

  Vector<T> & operator-(const T s) {
    // TODO: fix
    return this;
  }

  void printToMatrixMarketFile(string fileName) {
    // TODO: fix
  }
};
