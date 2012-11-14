#include "solver.h"
#include "array_vector.cpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  cout << "Hello World!" << endl;

  ArrayVector<int> av1(10);
  av1.setValue(0, 1);
  av1.setValue(5, 4);
  ArrayVector<int> av2(10);
  av2.setValue(5, 2);

  cout << "av1  ";
  av1.printToMatrixMarketFile("asdf");
  cout << "av2  ";
  av2.printToMatrixMarketFile("asdf");

  //ArrayVector<int> av3(10);
  ArrayVector<int> av3(av1 + av2);
  cout << "av3  ";
  av3.printToMatrixMarketFile("asdf");

  return 0;
}
