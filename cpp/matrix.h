#ifndef MATRIX_H
#define MATRIX_H

#include "matrix_market_handler.h"
#include "vector.h"

/**
 * An interface for matrices
 */
template <typename T>
class Matrix : public MatrixMarketHandler {

  public:
  /**
   * destructor for passing pointers when using interfaces
   * instead of concrete classes
   */
  virtual ~Matrix() {}

  /**
   * @return the number of horizontal elements
   */
  virtual unsigned int getM() const = 0;

  /**
   * @return the number of vertical elements
   */
  virtual unsigned int getN() const = 0;

  /**
   * @return the element at the given index
   */
  virtual T getValue(unsigned int column, unsigned int row) const = 0;

  /**
   * set the element at the given index (int) to the given value (T)
   */
  virtual void setValue(unsigned int column, unsigned int row, T value) = 0;

  /**
   * result = (this matrix) * v
   */
  virtual void multiply(Vector<T> &, Vector<T> &result) const = 0;
};

#endif

