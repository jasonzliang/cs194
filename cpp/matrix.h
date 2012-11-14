#ifndef MATRIX_H
#define MATRIX_H

#include "printable_element_container.h"
#include "vector.h"

/**
 * An interface for matrices
 */
template <typename T>
class Matrix : public PrintableElementContainer {

  public:
  /**
   * destructor for passing pointers when using interfaces
   * instead of concrete classes
   */
  virtual ~Matrix() {}

  /**
   * @return the number of horizontal elements
   */
  virtual int getM() const = 0;

  /**
   * @return the number of vertical elements
   */
  virtual int getN() const = 0;

  /**
   * @return the element at the given index
   */
  virtual T getValue(int column, int row) const = 0;

  /**
   * set the element at the given index (int) to the given value (T)
   */
  virtual void setValue(int column, int row, T value) = 0;

  /**
   * result = (this matrix) * v
   */
  virtual void multiply(Vector<T> &, Vector<T> &result) const = 0;
};

#endif

