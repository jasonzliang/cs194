#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
#include "cl.hpp"
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
using namespace cl;

float fnan = 0.0/0.0;
int inan = -1;

float *readVectorFromMatrixMarketFile(string fileName, int *size) {
  unsigned int row, col, maxSize = 0;
  float val;
  float *values;
  std::string line;

  std::ifstream mmFile(fileName.c_str());

  if (mmFile.is_open()) {
    while (mmFile.good()) {
      getline(mmFile, line);

      if (line[0] == '%') {
	continue;
      }
      std::stringstream lineStream(line);
      lineStream >> row >> col  >> val;
      if (maxSize == 0) {
	maxSize = col + 1;
	std::cout << "Vector size: " << maxSize << std::endl;
	values = new float[maxSize];
	values[0] = fnan;
	size[0] = maxSize;
	continue;
      }
      values[col] = val;
    }
    mmFile.close();
  }
  return values;
}


float dotProduct(float *vec1, float *vec2, int vSize) {
  double temp = 0.0;
  for (unsigned int i=0; i<vSize; i++) {
    temp += vec1[i] * vec2[i];
  }
  return temp;
}

float norm(float *vec, int vSize) {
  return sqrt(dotProduct(vec, vec, vSize));
}

typedef struct element {
  int index;
  float value;
  struct element *next;
} element;

element *readMatrixFromMatrixMarketFile(string fileName, int *size) {
  unsigned int row, col, maxSize = 0;
  float val;
  element *values;
  std::string line;

  std::ifstream mmFile(fileName.c_str());

  if (mmFile.is_open()) {
    while (mmFile.good()) {
      getline(mmFile, line);

      if (line[0] == '%') {
	continue;
      }
      std::stringstream lineStream(line);
      lineStream >> row >> col  >> val;
      if (maxSize == 0) {
	maxSize = col + 1;
	std::cout << "Matrix Size: " << maxSize << std::endl;
	values = new element[maxSize];
	for (unsigned int i=0 ; i<maxSize; i++) {
	  values[i].index = inan;
	  values[i].value = fnan;
	  values[i].next = NULL;
	}
	size[0] = maxSize;
	continue;
      }
      element *current = &values[row];
      while ((current->index != inan) && (current->value != fnan)) {
	current = current->next;
      }
      current->index = col;
      current->value = val;
      current->next = new element;
      (current->next)->index = inan;
      (current->next)->value = fnan;
      (current->next)->next = NULL;
    }
    mmFile.close();
  }
  return values;
}

int main(int argc, char **argv) {
  char *kernel_file = new char[0];
  kernel_file = "vector_operations_kernel.cl";

  int *size = new int[1];
  float *v = readVectorFromMatrixMarketFile("../test_vector.mtx", size);
  int vSize = size[0];
  free(size);
  std::cout << "Done reading the vector\n";

  size = new int[1];
  element *m = readMatrixFromMatrixMarketFile("../test_matrix.mtx", size);
  int mSize = size[0];
  free(size);
  int *matrixRowCount = new int[mSize];
  int maxCount = 0;
  for (int row=0; row<mSize; row++) {
    element current = m[row];
    int count = 0;
    while ((current.index != inan) && (current.value != fnan)) {
      count++;
      current = *(current.next);
    }
    matrixRowCount[row] = count;
    if (count > maxCount) {
      maxCount = count;
    }
  }
  int *matrixMaxRowCount = new int[1];
  matrixMaxRowCount[0] = maxCount;
  float *matrix = new float[mSize * maxCount];
  int *matrixIndexes = new int[mSize * maxCount];
  for (int row=1; row<mSize; row++) {
    element current = m[row];
    int count = row * maxCount;
    while ((current.index != inan) && (current.value != fnan)) {
      matrix[count] = current.value;
      matrixIndexes[count] = current.index;
      count++;
      current = *(current.next);
    }
  }
  std::cout << "Done reading the matrix (largest row has " << maxCount << " elements)\n";

  // create a zero vector to use for initialization
  float *zero = new float[vSize];
  for (int i=0; i<vSize; i++) {
    zero[i] = 0.0;
  }

  try { 
    // Get available platforms
    vector<Platform> platforms;
    Platform::get(&platforms);
 
    // Select the default platform and create a context using this platform and the GPU
    cl_context_properties cps[3] = { 
      CL_CONTEXT_PLATFORM, 
      (cl_context_properties)(platforms[0])(), 
      0 
    };
    Context context( CL_DEVICE_TYPE_GPU, cps);
 
    // Get a list of devices on this platform
    vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
 
    // Create a command queue and use the first device
    CommandQueue queue = CommandQueue(context, devices[0]);
 
    // Read source file
    std::ifstream sourceFile(kernel_file);
    std::string sourceCode(
			   std::istreambuf_iterator<char>(sourceFile),
			   (std::istreambuf_iterator<char>()));
    Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));
 
    // Make program of the source code in the context
    Program program = Program(context, source);
 
    // Build program for these specific devices
    program.build(devices);
 
    // Create memory buffers
    std::cout << "Creating buffers for matrix\n";
    Buffer bufferMatrix = Buffer(context, CL_MEM_READ_ONLY, mSize * maxCount * sizeof(float));
    Buffer bufferMatrixIndexes = Buffer(context, CL_MEM_READ_ONLY, mSize * maxCount * sizeof(int));
    Buffer bufferRowCounts = Buffer(context, CL_MEM_READ_ONLY, mSize * sizeof(int));
    Buffer bufferMatrixMaxRowCount = Buffer(context, CL_MEM_READ_ONLY, sizeof(int));

    std::cout << "Creating buffers for vectors\n";
    Buffer bufferB = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferX = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferR = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferY = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferZ = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferTemp = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferTempSingleFloat = Buffer(context, CL_MEM_READ_WRITE, sizeof(float));
 
    // Copy data into buffers
    std::cout << "Populating matrix buffers\n";
    queue.enqueueWriteBuffer(bufferMatrix, CL_TRUE, 0, mSize * maxCount * sizeof(float), matrix);
    queue.enqueueWriteBuffer(bufferMatrixIndexes, CL_TRUE, 0, mSize * maxCount * sizeof(int), matrixIndexes);
    queue.enqueueWriteBuffer(bufferRowCounts, CL_TRUE, 0, mSize * sizeof(int), matrixRowCount);
    queue.enqueueWriteBuffer(bufferMatrixMaxRowCount, CL_TRUE, 0, sizeof(int), matrixMaxRowCount);
    std::cout << "Populating vector buffers\n";
    queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, vSize * sizeof(float), v);
    queue.enqueueWriteBuffer(bufferX, CL_TRUE, 0, vSize * sizeof(float), zero);
    queue.enqueueWriteBuffer(bufferR, CL_TRUE, 0, vSize * sizeof(float), zero);
    queue.enqueueWriteBuffer(bufferY, CL_TRUE, 0, vSize * sizeof(float), zero);
    queue.enqueueWriteBuffer(bufferZ, CL_TRUE, 0, vSize * sizeof(float), zero);
    queue.enqueueWriteBuffer(bufferR, CL_TRUE, 0, vSize * sizeof(float), zero);
    queue.enqueueWriteBuffer(bufferTemp, CL_TRUE, 0, vSize * sizeof(float), zero);

    // set up the ranges
    NDRange global(vSize);
    NDRange local(1);



    // SOLVER STUFF
    float tolerance = 1e-6;

    // pre-iteration stuff
    float *s = new float[1];
    float *t = new float[1];
    float *temp = new float[1];
    float *result1 = new float[vSize];
    float *result2 = new float[vSize];
    
    // multiply(A, b, r);
    Kernel kernel01(program, "multiply");
    kernel01.setArg(0, bufferMatrix);
    kernel01.setArg(1, bufferMatrixIndexes);
    kernel01.setArg(2, bufferB);
    kernel01.setArg(3, bufferR);
    kernel01.setArg(4, bufferRowCounts);
    kernel01.setArg(5, bufferMatrixMaxRowCount);
    queue.enqueueNDRangeKernel(kernel01, NullRange, global, local);

    // increaseBy(r, b);
    Kernel kernel02(program, "increaseBy");
    kernel02.setArg(0, bufferR);
    kernel02.setArg(1, bufferB);
    queue.enqueueNDRangeKernel(kernel02, NullRange, global, local);

    // multiply(r, -1.0, y);
    Kernel kernel03(program, "vector_mult");
    temp[0] = -1.0;
    queue.enqueueWriteBuffer(bufferTempSingleFloat, CL_TRUE, 0, sizeof(float), temp);
    kernel03.setArg(0, bufferR);
    kernel03.setArg(1, bufferTempSingleFloat);
    kernel03.setArg(2, bufferY);
    queue.enqueueNDRangeKernel(kernel03, NullRange, global, local);

    // multiply(A, y, z);
    Kernel kernel04(program, "multiply");
    kernel04.setArg(0, bufferMatrix);
    kernel04.setArg(1, bufferMatrixIndexes);
    kernel04.setArg(2, bufferY);
    kernel04.setArg(3, bufferZ);
    kernel04.setArg(4, bufferRowCounts);
    kernel04.setArg(5, bufferMatrixMaxRowCount);
    queue.enqueueNDRangeKernel(kernel04, NullRange, global, local);

    // s = dotProduct(y, z);
    queue.enqueueReadBuffer(bufferY, CL_TRUE, 0, vSize * sizeof(float), result1);
    queue.enqueueReadBuffer(bufferZ, CL_TRUE, 0, vSize * sizeof(float), result2);
    s[0] = dotProduct(result1, result2, vSize);

    // t = dotProduct(r, y) / s;
    queue.enqueueReadBuffer(bufferR, CL_TRUE, 0, vSize * sizeof(float), result2);
    t[0] = dotProduct(result1, result2, vSize) / s[0];

    // multiply(y, t, x);
    Kernel kernel05(program, "vector_mult");
    queue.enqueueWriteBuffer(bufferTempSingleFloat, CL_TRUE, 0, sizeof(float), t);
    kernel05.setArg(0, bufferY);
    kernel05.setArg(1, bufferTempSingleFloat);
    kernel05.setArg(2, bufferX);
    queue.enqueueNDRangeKernel(kernel05, NullRange, global, local);

    // reduceBy(x, b);
    Kernel kernel06(program, "reduceBy");
    kernel06.setArg(0, bufferX);
    kernel06.setArg(1, bufferB);
    queue.enqueueNDRangeKernel(kernel06, NullRange, global, local);

    for (unsigned int k=0; k<vSize; k++) {
      // multiply(z, t, temp);
      Kernel kernelL1(program, "vector_mult");
      queue.enqueueWriteBuffer(bufferTempSingleFloat, CL_TRUE, 0, sizeof(float), t);
      kernelL1.setArg(0, bufferZ);
      kernelL1.setArg(1, bufferTempSingleFloat);
      kernelL1.setArg(2, bufferTemp);
      queue.enqueueNDRangeKernel(kernelL1, NullRange, global, local);

      // reduceBy(r, temp);
      Kernel kernelL2(program, "reduceBy");
      kernelL2.setArg(0, bufferR);
      kernelL2.setArg(1, bufferTemp);
      queue.enqueueNDRangeKernel(kernelL2, NullRange, global, local);

      // if (norm(r) < tolerance) { break; }
      queue.enqueueReadBuffer(bufferR, CL_TRUE, 0, vSize * sizeof(float), result1);
      if (norm(result1, vSize) < tolerance) { break; }

      // double B = dotProduct(r, z) / s;
      queue.enqueueReadBuffer(bufferZ, CL_TRUE, 0, vSize * sizeof(float), result2);
      temp[0] = dotProduct(result1, result2, vSize);

      // scaleBy(y, B);
      Kernel kernelL3(program, "scaleBy");
      queue.enqueueWriteBuffer(bufferTempSingleFloat, CL_TRUE, 0, sizeof(float), temp);
      kernelL3.setArg(0, bufferY);
      kernelL3.setArg(1, bufferTempSingleFloat);
      queue.enqueueNDRangeKernel(kernelL3, NullRange, global, local);

      // reduceBy(y, r);
      Kernel kernelL4(program, "reduceBy");
      kernelL4.setArg(0, bufferY);
      kernelL4.setArg(1, bufferR);
      queue.enqueueNDRangeKernel(kernelL4, NullRange, global, local);

      // multiply(A, y, z);
      Kernel kernelL5(program, "multiply");
      kernelL5.setArg(0, bufferMatrix);
      kernelL5.setArg(1, bufferMatrixIndexes);
      kernelL5.setArg(2, bufferY);
      kernelL5.setArg(3, bufferZ);
      kernelL5.setArg(4, bufferRowCounts);
      kernelL5.setArg(5, bufferMatrixMaxRowCount);
      queue.enqueueNDRangeKernel(kernelL5, NullRange, global, local);

      // s = dotProduct(y, z);
      queue.enqueueReadBuffer(bufferY, CL_TRUE, 0, vSize * sizeof(float), result1);
      queue.enqueueReadBuffer(bufferZ, CL_TRUE, 0, vSize * sizeof(float), result2);
      s[0] = dotProduct(result1, result2, vSize);

      // t = dotProduct(r, y) / s;
      queue.enqueueReadBuffer(bufferR, CL_TRUE, 0, vSize * sizeof(float), result2);
      t[0] = dotProduct(result1, result2, vSize) / s[0];

      // multiply(y, t, temp);
      Kernel kernelL6(program, "vector_mult");
      queue.enqueueWriteBuffer(bufferTempSingleFloat, CL_TRUE, 0, sizeof(float), t);
      kernelL6.setArg(0, bufferY);
      kernelL6.setArg(1, bufferTempSingleFloat);
      kernelL6.setArg(2, bufferTemp);
      queue.enqueueNDRangeKernel(kernelL6, NullRange, global, local);

      // increaseBy(x, temp);
      Kernel kernelL7(program, "increaseBy");
      kernelL7.setArg(0, bufferX);
      kernelL7.setArg(1, bufferTemp);
      queue.enqueueNDRangeKernel(kernelL7, NullRange, global, local);

    }
 
    // Read buffer C into a local list
    float *C = new float[vSize];
    queue.enqueueReadBuffer(bufferX, CL_TRUE, 0, vSize * sizeof(float), C);
 
    for(int i = 1; i < 4; i ++)
      std::cout << v[i] << " ~= " << C[i] << std::endl; 

    free(v);
    free(matrix);
    free(matrixIndexes);
    free(matrixRowCount);
    free(zero);

  } catch(Error error) {
    std::cout << error.what() << "(" << error.err() << ")" << std::endl;
  }
 
  return 0;

}
