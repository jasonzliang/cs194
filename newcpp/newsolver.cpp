#include <iostream>
#include <map>
#include <iterator>
#include <omp.h>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
//template <typename T>

#define DEFAULT_TOLERANCE 1e-6

/*structs -----------------------------------------------------*/

typedef struct {
	double *values;
	unsigned int size;
} array_vector;

typedef struct {
	map<unsigned int, double> *values;
	unsigned int M;
	unsigned int N;
} shm;

/* vector operations ------------------------------------------*/

void resizeVector(array_vector* oldvector, const unsigned int newSize) {
	oldvector->size = newSize;
	oldvector->values = new double[newSize];
}

void setValue(array_vector* myVec, const unsigned int i, const double value) {
	myVec->values[i] = value;
}

double dotProduct(const array_vector* vec1, const array_vector* vec2) {
	double temp = 0.0;
	for (unsigned int i=0; i<vec1->size; i++) {
		temp += vec1->values[i] * vec2->values[i];
	}
	return temp;
}

void increaseBy(array_vector* vec1, const array_vector* vec2) {
	for (unsigned int i=0; i<vec1->size; i++) {
		vec1->values[i] += vec2->values[i];
	}
}

void reduceBy(array_vector* vec1, const array_vector* vec2) {
	for (unsigned int i=0; i<vec1->size; i++) {
		vec1->values[i] -= vec2->values[i];
	}
}

void scaleBy(array_vector* vec1, const double s) {
	for (unsigned int i=0; i<vec1->size; i++) {
		vec1->values[i] *= s;
	}
}

void add(const array_vector* vec1, const array_vector* vec2, array_vector* vec3) {
	for (unsigned int i=0; i<vec1->size; i++) {
		vec3->values[i] = vec1->values[i] + vec2->values[i];
	}
}

void subtract(const array_vector* vec1, const array_vector* vec2, array_vector* vec3) {
	for (unsigned int i=0; i<vec1->size; i++) {
		vec3->values[i] = vec1->values[i] - vec2->values[i];
	}
}

void multiply(const array_vector* vec1, const double s, array_vector* vec3) {
	for (unsigned int i=0; i<vec1->size; i++) {
		vec3->values[i] = s * vec1->values[i];
	}
}

double norm(array_vector* myVec) {
  return sqrt(dotProduct(myVec, myVec));
}

array_vector *readFromMatrixMarketFile_arrayVector(string fileName) {
	unsigned int row, col;
	double val; // really this has to be double or float
	string line;

	ifstream mmFile(fileName.c_str());

	array_vector *myArrayVector = new array_vector;
	myArrayVector->size = 0;

	if (mmFile.is_open()) {
		while (mmFile.good()) {
			getline(mmFile, line);

			if (line[0] == '%') {
				continue;
			}
			stringstream lineStream(line);
			lineStream >> row >> col  >> val;
			if (myArrayVector->size == 0) { // not allocated yet
				myArrayVector->size = col + 1;
				cout << "Vector size: " << myArrayVector->size << endl;
				//				values = new T[size];
				myArrayVector->values = new double[myArrayVector->size];
				continue;
			}
			myArrayVector->values[col] = val;
		}
		mmFile.close();
	}

	return myArrayVector;
}

void printToMatrixMarketFile(string fileName, array_vector* myArrayVector) {
	unsigned int numEntries = 0;
	for (unsigned int i = 0; i < myArrayVector->size; i++) {
		if (myArrayVector->values[i] != 0.0) {
			numEntries++;
		}
	}

	ofstream mmFile (fileName.c_str());

	if (mmFile.is_open()) {

		mmFile << "%%MatrixMarket matrix coordinate real general" << endl;
		mmFile << "%" << endl;
		mmFile << "1" << " " << myArrayVector->size - 1 << " " <<numEntries << endl;

		for (unsigned int i=0; i<myArrayVector->size; i++) {
			if (myArrayVector->values[i] != 0) {
				mmFile << "1" << " " << i << " " << myArrayVector->values[i] << endl;
			}
		}

		mmFile.close();
	} else {
		cout << "Problem writing matrix market file" << endl;
	}
}

/* matrix operations --------------------------------------*/

void multiply(const shm* myMat, const array_vector* myVec, array_vector* result) {
	int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
	omp_set_num_threads(nprocs);

	//cout << "check2" << endl;
	typename map<unsigned int, double>::iterator itr;
	//cout << "check3" << endl;
	//cout << myMat->M << endl;
	#pragma omp parallel for private(itr) // probably should use blocking
	for (unsigned int row=0; row<myMat->M; row++) {
		//cout << "check4" << endl;
		double temp = 0.0;
		for (itr = myMat->values[row].begin(); itr != myMat->values[row].end(); ++itr) {
			temp += myVec->values[itr->first] * itr->second;
		}
		result->values[row] = temp;
	}
}

//void multiply(Vector<T> &v, Vector<T> &result) const {
//	int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
//	//cout << "using " << nprocs << " threads" << endl;
//	omp_set_num_threads(nprocs);
//
//	typename map<unsigned int, T>::iterator itr;
//	#pragma omp parallel for private(itr) // probably should use blocking
//	for (unsigned int row=0; row<getN(); row++) {
//		T temp = 0;
//		for (itr = values[row].begin(); itr != values[row].end(); ++itr) {
//			temp += v.getValue(itr->first) * itr->second;
//		}
//		result.setValue(row, temp);
//	}
//}

shm *readFromMatrixMarketFile_shm(string fileName) {
	unsigned int row, col;
	double val; // really this has to be double or float

	string line;
	ifstream mmFile (fileName.c_str());

	shm *myShm = new shm;
	myShm->M = 0;
	myShm->N = 0;

	if (mmFile.is_open()) {
		while (mmFile.good()) {
			getline(mmFile, line);

			if (line[0] == '%') {
				continue;
			}
			stringstream lineStream(line);
			lineStream >> row >> col  >> val;

			if (myShm->M == 0 && myShm->N == 0) { // not allocated yet
				myShm->M = row + 1;
				myShm->N = col + 1;
				myShm->values = new map<unsigned int, double>[myShm->M];
				cout << "Matrix size: " << myShm->M << ", " << myShm->N << endl;
				continue;
			}
			myShm->values[row][col] = val;
		}

		mmFile.close();
	}
	return myShm;
}

void printToMatrixMarketFile(string fileName, shm* myShm) {
	unsigned int numEntries = 0;

	typename map<unsigned int, double>::iterator itr;

	for (unsigned int row=0; row<myShm->M; row++) {
		for (itr = myShm->values[row].begin(); itr != myShm->values[row].end(); ++itr) {
			numEntries++;
		}
	}

	ofstream mmFile (fileName.c_str());

	if (mmFile.is_open()) {
		mmFile << "%%MatrixMarket matrix coordinate real general" << endl;
		mmFile << "%" << endl;
		mmFile << myShm->M - 1 << " " << myShm->N - 1 << " " << numEntries << endl;

		for (unsigned int row=0; row<myShm->M; row++) {
			for (itr = myShm->values[row].begin(); itr != myShm->values[row].end(); ++itr) {
				mmFile << row << " " << itr->first << " " << itr->second << endl;
			}
		}
		mmFile.close();
	} else {
		cout << "Problem writing matrix market file" << endl;
	}
}

/* solver -------------------------------------------------*/

void solve(array_vector *x, shm* A, array_vector *b, float tolerance) {
	array_vector* r = new array_vector;
	resizeVector(r, b->size);

	array_vector* y = new array_vector;
	resizeVector(y, b->size);

	array_vector* z = new array_vector;
	resizeVector(z, b->size);

	double s;
	double t;

	//cout << "check1" << endl;

	array_vector* temp = new array_vector;
	resizeVector(temp, b->size);

	multiply(A, b, r);
	increaseBy(r, b);
	multiply(r, -1.0, y);
	multiply(A, y, z);
	s = dotProduct(y, z);
	t = dotProduct(r, y) / s;
	multiply(y, t, x);
	reduceBy(x, b);

	unsigned int maxIters = b->size;
	for (unsigned int k=0; k<maxIters; k++) {
		multiply(z, t, temp);
		reduceBy(r, temp);

		cout << "Residual (" << k << "/" << maxIters << "): " << norm(r) << endl;
		if (norm(r) < tolerance) {
			break;
		}
		double B = dotProduct(r, z) / s;
		scaleBy(y, B);
		reduceBy(y, r);
		multiply(A, y, z);
		s = dotProduct(y, z);
		t = dotProduct(r, y) / s;
		multiply(y, t, temp);
		increaseBy(x, temp);
	}
}

int main(int argc, char **argv) {

	array_vector* myVec = readFromMatrixMarketFile_arrayVector("../vector.txt.mtx");
	shm* myMat = readFromMatrixMarketFile_shm("../matrix.txt.mtx");

  array_vector* result = new array_vector;
	resizeVector(result, myVec->size);

	solve(result, myMat, myVec, DEFAULT_TOLERANCE);

  printToMatrixMarketFile("newsolver_result.mtx.txt", result);

  return 0;
}
