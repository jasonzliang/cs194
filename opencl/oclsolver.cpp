#define __NO_STD_VECTOR
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "cl.hpp"
using namespace std;

int main(int argc, char **argv) {
  ifstream sourceFile("helloworld_kernel.cl");
  string sourceCode(istreambuf_iterator<char>(sourceFile),
		    (istreambuf_iterator<char>()));

  cl::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  string platformVendor;
  platforms[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
  cout << "Plaform: " << platformVendor << endl;
  cl_context_properties cps[3] = {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)(platforms[0])(),
    0
  };
  cl::Context context( CL_DEVICE_TYPE_GPU, cps);
  cl::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
  cl::CommandQueue queue = cl::CommandQueue(context, devices[0]);
  cl::Program::Sources source(1, make_pair(sourceCode.c_str(), sourceCode.length() + 1));
  cl::Program program = cl::Program(context, source);
  program.build(devices);

  // just for calculating buffer size
  unsigned int strLength = 10 + 1;
  cl::Buffer clOut(context, CL_MEM_WRITE_ONLY, strLength);
  cl::Kernel kernel(program, "hello");
  kernel.setArg(0, clOut);

  cl::NDRange global(strLength);
  cl::NDRange local(1, 1);
  queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
  char *hostOut = new char[strLength];
  queue.enqueueReadBuffer(clOut, CL_TRUE, 0, strLength, hostOut);

  cout << "Output :" << hostOut << endl;
}
