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
  virtual int getSize() const = 0;

  /**
   * @return the element at the given index
   */
  virtual T getValue(int) const = 0;

  /**
   * set the element at the given index (int) to the given value (T)
   */
  virtual void setValue(int, T) = 0;

  /**
   * @return the scalar dot product of this and the argument Vector
   */
  virtual T dotProduct(Vector<T> &) const = 0;

  /**
   * @return the dot product of this vector with itself
   */
  T norm() const {
    return dotProduct(*this);
  }

  /**
   * mutate this vector by adding element-wise with the argument Vector
   * @return this vector
   */
  virtual void increaseBy(Vector<T> &) = 0;

  /**
   * mutate this vector by stracting the argument Vector's elements from
   * this vector (as in: this vector - argument vector)
   * @return this vector
   */
  virtual void reduceBy(Vector<T> &) = 0;

  /**
   * mutate this vector by multiplying each element with the argument
   * @return this vector
   */
  virtual void scaleBy(T) = 0;

  /**
   * result = (this vector) + v2
   */
  virtual void add(const Vector<T> &v2, Vector<T> &result) const = 0;

  /**
   * result = (this vector) - v2
   */
  virtual void subtract(const Vector<T> &, Vector<T> &result) const = 0;

  /**
   * result = (this vector) * s
   */
  virtual void multiply(const T, Vector<T> &result) const = 0;

  const T operator[](const int i) const {
    return getValue(i);
  }
};

#endif

