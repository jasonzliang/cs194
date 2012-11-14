#include "solver.h"
#include "array_vector.cpp"
#include "sparse_hashed_matrix.cpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  cout << "Hello World!" << endl;

  ArrayVector<int> av(10);
  av.setValue(0,1);
  av.setValue(5,4);
  av.printToMatrixMarketFile("testout.txt");

  SparseHashedMatrix<int> shm(5, 5);
  shm.setValue(0, 0, 2);
  shm.setValue(3, 2, 3);
  shm.print();
  ArrayVector<int> mult(5);
  mult.setValue(0, 1);
  mult.setValue(1, 2);
  mult.setValue(2, 3);
  mult.setValue(3, 4);
  mult.setValue(4, 5);
  mult.print();
  ArrayVector<int> res(5);
  shm.multiply(mult, res);
  res.print();

  return 0;
}
