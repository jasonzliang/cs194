#define __NO_STD_VECTOR
#include <stdio.h>
#include <stdlib.h>


#include "cl.hpp" // Got this from http://www.khronos.org/registry/cl/ 'coz I can't find it 

#include <iostream>
#include <fstream>
#include <string>
using namespace cl;

// just following this http://www.thebigblob.com/using-the-cpp-bindings-for-opencl/, guess the
// rest of the code is pretty self explanatory
// Actual docs of C++ bindings: http://www.khronos.org/registry/cl/specs/opencl-cplusplus-1.1.pdf
// Pretty helpful (and much shorter) articles: http://enja.org/2010/07/20/adventures-in-opencl-part-1-5-cpp-bindings/
// http://developer.amd.com/tools/heterogeneous-computing/amd-accelerated-parallel-processing-app-sdk/introductory-tutorial-to-opencl/
int main(int argc, char **argv) {
  std::string kernel_str = 
    "__constant char hw[] = \"Hello WOrld\\n\""
    "__kernel void hello(__global char* out){"
    "unsigned int tid = get_global_id(0);"
    "out[tid] = hw[tid];"
    "}";
    
 vector<Platform> platforms;
  Platform::get(&platforms);

  std::string platformVendor;
  platforms[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
  std::cout << "Plaform: " << platformVendor << std::endl;

  cl_context_properties cps[3] = {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)(platforms[0])(),
    0
  };

 
  Context context( CL_DEVICE_TYPE_GPU, cps);
  vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
  
  CommandQueue queue = CommandQueue(context, devices[0]);

  Program::Sources source(1, std::make_pair(kernel_str.c_str(), kernel_str.length() + 1));

  Program program = Program(context, source);

  program.build(devices);

  // just for calculating buffer size
  std::string hw("Hello World\n");
  char *hostOut = new char[hw.length() + 1];
  Buffer clOut(context, CL_MEM_WRITE_ONLY, hw.length() + 1);

  Kernel kernel(program, "hello");
  kernel.setArg(0, clOut);

  NDRange global(hw.length() + 1);
  NDRange local(1, 1);
  queue.enqueueNDRangeKernel(kernel, NullRange, global, local);

  queue.enqueueReadBuffer(clOut, CL_TRUE, 0, hw.length() + 1, hostOut);

  std::cout << "Output :" << hostOut << std::endl;
    
}

