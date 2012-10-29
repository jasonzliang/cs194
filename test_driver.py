"""
Compares output of both solvers.

Assumes input is of Matrix Market (mtx) format

"""
import sys
from scipy.io import mmread, mmwrite
import numpy


def compare_output(file1, file2, rtol=1.0000000000000001e-05, atol=1e-08):
    m1 = mmread(file1)
    m2 = mmread(file2)

    # absolute(a - b) <= (atol + rtol * absolute(b))
    return numpy.allclose(m1.toarray(), m2.toarray(), rtol=rtol, atol = atol)
    

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Please specify input files"
        exit()
  
    threshold = 0

    file1 = sys.argv[1]
    file2 = sys.argv[2]
    
    if len(sys.argv) == 4:
        threshold = float(sys.argv[3])

    print "Comparing %s and %s" % (file1, file2)
    compare_output(file1, file2, threshold=threshold)
    
