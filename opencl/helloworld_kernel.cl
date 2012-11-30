__constant char hw[] = "hey there\n";

__kernel void hello(__global char* out, int x) {
  unsigned int tid = get_global_id(0);
  out[tid] = hw[tid];
}
