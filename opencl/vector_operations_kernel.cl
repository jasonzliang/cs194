__kernel void vector_add(__global float *A, __global float *B, __global float *C) {
    int i = get_global_id(0);
    C[i] = A[i] + B[i];
}

__kernel void vector_sub(__global float *A, __global float *B, __global float *C) {
    int i = get_global_id(0);
    C[i] = A[i] - B[i];
}

__kernel void increaseBy(__global float *vec1, __global const float *vec2) {
   int i = get_global_id(0);
   vec1[i] += vec2[i];
}

__kernel void reduceBy(__global float *vec1, __global const float *vec2) {
   int i = get_global_id(0);
   vec1[i] -= vec2[i];
}

__kernel void scaleBy(__global float *vec1, __global const float *s) {
   int i = get_global_id(0);
   vec1[i] *= s[0];
}

typedef struct element {
  unsigned int index;
  float value;
  struct element *next;
} element;

__kernel void multiply(__global element *matrix, __global float *operandVec, __global float *resultVec) {
  int i = get_global_id(0); // matrix row and vector element
  element current = matrix[i];
  float fnan = 0.0/0.0;
  int inan = 0/0;

  while ((current.index != inan) && (current.value != fnan)) {
    resultVec[i] += current.value * operandVec[i];
    current = *(current.next);
  }
}
