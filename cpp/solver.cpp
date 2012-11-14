#include "solver.h"
#include "array_vector.cpp"
#include <iostream>

using namespace std;



int main(int argc, char **argv) {
  cout << "Hello World!" << endl;

  // ArrayVector<int> av(10);
  // av.setValue(0,1);
  // av.setValue(5,4);
  // av.printToMatrixMarketFile("testout.txt");

  // test reading file - should print in console
  ArrayVector<float> avRead("../vector.txt.mtx");


  return 0;
}
