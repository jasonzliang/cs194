#ifndef MATRIX_MARKET_HANDLER_H
#define MATRIX_MARKET_HANDLER_H

#include <iostream>

using namespace std;

/**
 * An interface for classes that can print its elements
 */
class MatrixMarketHandler {

  public:
    virtual ~MatrixMarketHandler() {}
    virtual void printToMatrixMarketFile(string) = 0;
};

#endif
