#ifndef PRINTABLE_ELEMENT_CONTAINER_H
#define PRINTABLE_ELEMENT_CONTAINER_H

#include <iostream>

using namespace std;

/**
 * An interface for classes that can print its elements
 */
class PrintableElementContainer {

  public:
    virtual ~PrintableElementContainer() {}
    virtual void printToMatrixMarketFile(string) = 0;
};

#endif
