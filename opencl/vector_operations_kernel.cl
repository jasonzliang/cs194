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

__kernel void multiply(__global float *matrix, __global int *matrixRowIndexes, __global float *operandVec, __global float *resultVec, __global int *numRowElements, __global int *numElementsInRow) {
  int i = get_global_id(0);
  for (int j=0; j<numRowElements[0]; j++) {
    int index = j + i * numElementsInRow[0];
    resultVec[j] += matrix[index] * operandVec[matrixRowIndexes[index]];
  }
}