#include "matrix.h"
#include "vector.h"
#include "matrix_market_handler.h"
#include <iostream>
#include <map>
#include <iterator>
#include <omp.h>

using namespace std;

template <typename T>
class SparseHashedMatrix : public Matrix<T> {
private:
	map<unsigned int, T> *values;
	unsigned int M;
	unsigned int N;

public:
	SparseHashedMatrix<T>() {
		M = 0;
		N = 0;
	}

	SparseHashedMatrix<T>(unsigned int m, unsigned int n) {
		M = m;
		N = n;
		values = new map<unsigned int, T>[N];
	}

	SparseHashedMatrix<T>(const SparseHashedMatrix<T> &m2) {
		M = m2.getM();
		N = m2.getN();

		typename map<unsigned int, T>::iterator itr;
		for (unsigned int row=0; row<getN(); row++) {
			for (itr = m2.values[row].begin(); itr != m2.values[row].end(); ++itr) {
				setValue(itr->first, row, itr->second);
			}
		}
	}

	SparseHashedMatrix<T>(string fileName) {
		M = 0;
		N = 0;
		readFromMatrixMarketFile(fileName);
	}

	~SparseHashedMatrix<T>() {
		delete[] values;
	}

	unsigned int getM() const {
		return M;
	}

	unsigned int getN() const {
		return N;
	}

	T getValue(unsigned int column, unsigned int row) const {
		if (values[row].find(column) == values[row].end()) {
			return 0;
		} else {
			return values[row].find(column)->second;
		}
	}

	void setValue(unsigned int column, unsigned int row, T value) {
		values[row][column] = value;
	}

	void multiply(Vector<T> &v, Vector<T> &result) const {
		int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
		//cout << "using " << nprocs << " threads" << endl;
		omp_set_num_threads(nprocs);

		typename map<unsigned int, T>::iterator itr;
		#pragma omp parallel for private(itr) // probably should use blocking
		for (unsigned int row=0; row<getN(); row++) {
			T temp = 0;
			for (itr = values[row].begin(); itr != values[row].end(); ++itr) {
				temp += v.getValue(itr->first) * itr->second;
			}
			result.setValue(row, temp);
		}
	}

	void printToMatrixMarketFile(string fileName) {
		unsigned int numEntries = 0;

		typename map<unsigned int, T>::iterator itr;

		for (unsigned int row=0; row<getN(); row++) {
			for (itr = values[row].begin(); itr != values[row].end(); ++itr) {
				numEntries++;
			}
		}

		ofstream mmFile (fileName.c_str());

		if (mmFile.is_open()) {
			mmFile << "%%MatrixMarket matrix coordinate real general" << endl;
			mmFile << "%" << endl;
			mmFile << getM()-1 << " " << getN()-1 << " " << numEntries << endl;

			for (unsigned int row=0; row<getN(); row++) {
				for (itr = values[row].begin(); itr != values[row].end(); ++itr) {
					mmFile << row << " " << itr->first << " " << itr->second << endl;
				}
			}

			mmFile.close();
		} else {
			cout << "Problem writing matrix market file" << endl;
		}
	}

	// seriously don't use this except when testing
	// with large matrices, things will go kaplooy
	void print() {
		for (unsigned int row=0; row<getN(); row++) {
			for (unsigned int column=0; column<getM(); column++) {
				cout << " " << getValue(column, row);
			}
			cout << endl;
		}
	}

	void readFromMatrixMarketFile(string fileName) {
		unsigned int row, col;
		T val; // really this has to be double or float

		string line;
		ifstream mmFile (fileName.c_str());

		if (mmFile.is_open()) {
			while (mmFile.good()) {
				getline(mmFile, line);

				if (line[0] == '%') {
					continue;
				}

				std::stringstream lineStream(line);

				lineStream >> row >> col  >> val;


				if (M == 0 && N == 0) { // not allocated yet
					N = row + 1;
					M = col + 1;
					values = new map<unsigned int, T>[N];
					cout << "Matrix size: " << M << ", " << N << endl;

					continue;
				}

				// Not too sure if I should swap row with col
				setValue(col, row, val);

				// TODO : remove. for checking only
				//cout << "Setting val: col " << col << ", row" << row << " val " << val << endl;
				//cout << getValue(col, row) << endl;
			}

			mmFile.close();
		}

	}


};
