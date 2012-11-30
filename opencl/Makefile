LIBS=-lOpenCL
CFLAGS=-O3
all:
	g++ $(CFLAGS) -o ocl_solver -I /usr/local/cuda-4.2/NVIDIA_GPU_Computing_SDK/OpenCL/common/inc ocl_solver.cpp $(LIBS)

clean:
	rm -rf ocl_solver *.o
