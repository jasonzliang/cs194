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

float norm(float *vec1, float *vec2, int vSize) {
  return sqrt(dotProduct(vec1, vec2, vSize));
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

  //int *size = new int[1];
  //float *v = readVectorFromMatrixMarketFile("../vector.txt.mtx", size);
  //int vSize = size[0];
  //free(size);
  int vSize = 10;
  float *v = new float[vSize];
  for (int i=0; i<vSize; i++) {
    v[i] = i/7.0;
  }
  int *size = new int[1];
  element *m = readMatrixFromMatrixMarketFile("../matrix.txt.mtx", size);
  int mSize = size[0];
  free(size);
  std::cout << "Done reading the matrix\n";
  //for (int row=0; row<4; row++) {
  //  element current = m[row];
  //  while ((current.index != inan) && (current.value != fnan)) {
  //    std::cout << "[" << row << "," << current.index << "]: " << current.value << std::endl;
  //    current = *(current.next);
  //  }
  //}


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
    Buffer bufferA = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferB = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
    Buffer bufferC = Buffer(context, CL_MEM_READ_WRITE, vSize * sizeof(float));
 
    // Copy lists A and B to the memory buffers
    queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, vSize * sizeof(float), v);
    queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, vSize * sizeof(float), v);
 
    // Make kernel
    Kernel kernel1(program, "vector_add");
    // Set arguments to kernel
    kernel1.setArg(0, bufferA);
    kernel1.setArg(1, bufferB);
    kernel1.setArg(2, bufferC);
 
    // Run the kernel on specific ND range
    NDRange global(vSize);
    NDRange local(1);
    queue.enqueueNDRangeKernel(kernel1, NullRange, global, local);

    // now subtract
    //Program program2 = Program(context, source);
    Kernel kernel2(program, "vector_sub");
    kernel2.setArg(0, bufferC);
    kernel2.setArg(1, bufferB);
    kernel2.setArg(2, bufferC);
    queue.enqueueNDRangeKernel(kernel2, NullRange, global, local);
 
    // Read buffer C into a local list
    float *C = new float[vSize];
    queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, vSize * sizeof(float), C);
 
    for(int i = 0; i < 10; i ++)
      std::cout << v[i] << " ~= " << C[i] << std::endl; 

    free(v);







  } catch(Error error) {
    std::cout << error.what() << "(" << error.err() << ")" << std::endl;
  }
 
  return 0;

}
