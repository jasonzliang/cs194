LIBS=-lOpenCL
CFLAGS=-O3
all:
	g++ $(CFLAGS) -o hello -I /usr/local/cuda-4.2/NVIDIA_GPU_Computing_SDK/OpenCL/common/inc helloworld.cpp $(LIBS)

clean:
	rm -rf hello  *.o
