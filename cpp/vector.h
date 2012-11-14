#ifndef VECTOR_H
#define VECTOR_H

#include "printable_element_container.h"

/**
 * An interface for vectors
 */
template <typename T>
class Vector : PrintableElementContainer {

  public:
  /**
   * destructor for passing pointers when using interfaces
   * instead of concrete classes
   */
  virtual ~Vector() {}

  /**
   * @return the size of this vector
   */
  virtual const int getSize() = 0;

  /**
   * @return the element at the given index
   */
  virtual const T getValue(int) = 0;

  /**
   * set the element at the given index (int) to the given value (T)
   */
  virtual void setValue(int, T) = 0;

  /**
   * @return the scalar dot product of this and the argument Vector
   */
  virtual const T dotProduct(Vector<T> &) = 0;

  /**
   * @return the dot product of this vector with itself
   */
  const T norm() {
    return dotProduct(*this);
  }

  /**
   * mutate this vector by adding element-wise with the argument Vector
   * @return this vector
   */
  virtual Vector<T> increaseBy(Vector<T> &) = 0;

  /**
   * mutate this vector by stracting the argument Vector's elements from
   * this vector (as in: this vector - argument vector)
   * @return this vector
   */
  virtual Vector<T> reduceBy(Vector<T> &) = 0;

  /**
   * mutate this vector by multiplying each element with the argument
   * @return this vector
   */
  virtual Vector<T> scaleBy(T) = 0;

  /**
   * @return a new vector that's (this vector) + (argument vector)
   */
  virtual Vector<T> & operator+(const Vector<T> &) = 0;

  /**
   * @return a new vector that's (this vector) - (argument vector)
   */
  virtual Vector<T> & operator-(const Vector<T> &) = 0;

  /**
   * @return a new vector that's (this vector)*argument
   */
  virtual Vector<T> & operator*(const T) = 0;

  T & operator[](const int i) {
    return getValue(i);
  }
};

#endif

