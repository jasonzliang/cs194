#include <iostream>
#include <map>
#include <omp.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;
//template <typename T>

#define DEFAULT_TOLERANCE 1e-6

/*structs -----------------------------------------------------*/

typedef struct {
	double *values;
	unsigned int size;
} array_vector;

struct element {
  unsigned int index;
  double value;
  struct element *next;
};

typedef struct {
	//map<unsigned int, double> *values;
	element **values;
	unsigned int M;
	unsigned int N;
} sm;

/*linked list operations --------------------------------------*/
element *find_by_index_or_return_last(unsigned int index, element *first) {
  element *current;
  current = first;
  while (current != NULL) {
    if ((current->index == index) || (current->next == NULL)) {
      return current;
    }
    current = current->next;
  }
  return current;
}

element *find_by_index(unsigned int index, element *first) {
  element *current;
  current = find_by_index_or_return_last(index, first);
  if (current->index == index) {
    return current;
  }
  return NULL;
}

element *add_or_update(unsigned int index, double value, element *first) {
  if (first == NULL) {
    first = new element;
    first->index = index;
    first->value = value;
    first->next = NULL;
    return first;
  }

  element *current = find_by_index_or_return_last(index, first);
  element *e;

  if (current->index == index) {
    current->value = value;
    e = current;
  } else {
    e = new element;
    e->index = index;
    e->value = value;
    e->next = NULL;
    current->next = e;
  }

  return e;
}

void print_elements(element *first) {
  element *current = first;
  while (current != NULL) {
    cout << "[" << current->index << "]: " << current->value << " (" << current << ")" << endl;

    current = current->next;
  }
}

unsigned int num_elements(element *first) {
  unsigned int count = 0;
  element *current = first;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

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

void multiply(const sm* myMat, const array_vector* myVec, array_vector* result) {
	int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
	omp_set_num_threads(nprocs);

	#pragma omp parallel for
	for (unsigned int row=0; row<myMat->M; row++) {
		element *current = myMat->values[row];
		double temp = 0.0;
		while (current != NULL) {
			if (isnan(myMat->values[row]->index) || isnan(myMat->values[row]->value)) {
				current = current->next;
				continue;
			}
			temp += myVec->values[current->index] * current->value;
			current = current->next;
		}
		result->values[row] = temp;
	}
}

sm *readFromMatrixMarketFile_sm(string fileName) {
	unsigned int row, col;
	double val;

	string line;
	ifstream mmFile (fileName.c_str());

	sm *mySm = new sm;
	mySm->M = 0;
	mySm->N = 0;

	if (mmFile.is_open()) {
		while (mmFile.good()) {
			getline(mmFile, line);

			if (line[0] == '%') {
				continue;
			}
			stringstream lineStream(line);
			lineStream >> row >> col  >> val;

			if (mySm->M == 0 && mySm->N == 0) { // not allocated yet
				mySm->M = row + 1;
				mySm->N = col + 1;
				mySm->values = new element*[mySm->M];
				// sentinel values for the elements NaN
				for (unsigned int row=0; row<mySm->M; row++) {
					mySm->values[row] = new element;
					mySm->values[row]->index = 0 / 0;
					mySm->values[row]->value = 0.0 / 0;
					mySm->values[row]->next = NULL;
				}
				cout << "Matrix size: " << mySm->M << ", " << mySm->N << endl;
				continue;
			}
			element *temp = add_or_update(col, val, mySm->values[row]);
			if (isnan(mySm->values[row]->value)) {
				mySm->values[row]->value = temp->value;
				mySm->values[row]->index = temp->index;
				mySm->values[row]->next = NULL;
			}
		}

		mmFile.close();
	}
	return mySm;
}

void printToMatrixMarketFile(string fileName, sm* mySm) {
	unsigned int numEntries = 0;

	typename map<unsigned int, double>::iterator itr;

	for (unsigned int row=0; row<mySm->M; row++) {
		numEntries += num_elements(mySm->values[row]);
	}

	ofstream mmFile (fileName.c_str());

	if (mmFile.is_open()) {
		mmFile << "%%MatrixMarket matrix coordinate real general" << endl;
		mmFile << "%" << endl;
		mmFile << mySm->M - 1 << " " << mySm->N - 1 << " " << numEntries << endl;

		for (unsigned int row=0; row<mySm->M; row++) {
			element *current = mySm->values[row];
			while (current != NULL) {
				mmFile << row << " " << current->index << " " << current->value << endl;
				current = current->next;
			}
		}
		mmFile.close();
	} else {
		cout << "Problem writing matrix market file" << endl;
	}
}

/* solver -------------------------------------------------*/

void solve(array_vector *x, sm* A, array_vector *b, float tolerance) {
	array_vector* r = new array_vector;
	resizeVector(r, b->size);

	array_vector* y = new array_vector;
	resizeVector(y, b->size);

	array_vector* z = new array_vector;
	resizeVector(z, b->size);

	double s;
	double t;

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
	time_t start_time = 0.0;
	start_time = time(NULL);
	array_vector* myVec = readFromMatrixMarketFile_arrayVector("../vector.txt.mtx");
	sm* myMat = readFromMatrixMarketFile_sm("../matrix.txt.mtx");
	time_t time_read = time(NULL) - start_time;

	array_vector* result = new array_vector;
	resizeVector(result, myVec->size);

	start_time = time(NULL);
	solve(result, myMat, myVec, DEFAULT_TOLERANCE);
	time_t time_solve = time(NULL) - start_time;

	start_time = time(NULL);
	printToMatrixMarketFile("llsolver_result.mtx.txt", result);
	time_t time_write = time(NULL) - start_time;

	// time stats
	cout << "Time to read: " << time_read << " [s]" << endl;
	cout << "Time to solve: " << time_solve << " [s]" << endl;
	cout << "Time to write: " << time_write << " [s]" << endl;

	return 0;
}
