LIBS=-lOpenCL
CFLAGS=-O3
all:
	g++ $(CFLAGS) -o oclsolver -I /usr/local/cuda-4.2/NVIDIA_GPU_Computing_SDK/OpenCL/common/inc oclsolver.cpp $(LIBS)

hello:
	g++ $(CFLAGS) -o helloworld -I /usr/local/cuda-4.2/NVIDIA_GPU_Computing_SDK/OpenCL/common/inc helloworld.cpp $(LIBS)

clean:
	rm -rf oclsolver helloworld *.o
