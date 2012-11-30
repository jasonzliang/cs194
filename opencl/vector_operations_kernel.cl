__kernel void vector_mult(__global const float *vec, __global const float *s, __global float *result) {
  int i = get_global_id(0);
  if (isnan(vec[i]) || isnan(s[0])) { return; }
  result[i] = vec[i] * s[0];
}

__kernel void increaseBy(__global float *vec1, __global const float *vec2) {
  int i = get_global_id(0);
  if (isnan(vec1[i]) || isnan(vec2[i])) { return; }
  vec1[i] += vec2[i];
}

__kernel void reduceBy(__global float *vec1, __global const float *vec2) {
  int i = get_global_id(0);
  if (isnan(vec1[i]) || isnan(vec2[i])) { return; }
  vec1[i] -= vec2[i];
}

__kernel void scaleBy(__global float *vec1, __global const float *s) {
  int i = get_global_id(0);
  if (isnan(vec1[i]) || isnan(s[0])) { return; }
  vec1[i] *= s[0];
}

__kernel void multiply(__global const float *matrix, __global const int *matrixRowIndexes, __global const float *operandVec, __global float *resultVec, __global const int *numRowElements, __global const int *numMaxElementsInRow) {
  int i = get_global_id(0);
  resultVec[i] = 0.0;
  for (int j=0; j<numRowElements[i]; j++) {
    int index = j + i * numMaxElementsInRow[0];
    resultVec[i] += matrix[index] * operandVec[matrixRowIndexes[index]];
  }
}
