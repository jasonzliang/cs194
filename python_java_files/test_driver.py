"""
Compares output of both solvers.
Usage:
python test_driver.py file1.mtx file2.mtx [1.000-e05] [1e-08]
                                            rtol        atol

Returns  absolute(a - b) <= (atol + rtol * absolute(b))


Assumes input is of Matrix Market (mtx) format

"""
import sys
import os
from scipy.io import mmread, mmwrite
import numpy
import pcgsolver_new


def compare_output(file1, file2, rtol=1.0000000000000001e-05, atol=1e-08):
    m1 = mmread(file1)
    m2 = mmread(file2)

    # absolute(a - b) <= (atol + rtol * absolute(b))
    return numpy.allclose(m1.toarray(), m2.toarray(), rtol=rtol, atol = atol)
    

if __name__ == "__main__":
#    if len(sys.argv) < 3:
#        print "Please specify input files"
#        exit()
  
    rtol = 1.0000000000000001e-05
    atol = 1e-08
    pythontime = pcgsolver_new.run()
    os.system('java -cp bin/ Driver > javatime.txt')
    f = open('javatime.txt', 'rb')
    javatime = float(f.readline().rstrip())
    os.system('rm javatime.txt')
    
    print "Python Time: ", pythontime
    print "Java Time: ", javatime
    
    if compare_output('python_solution.txt.mtx', 'java_solution.txt.mtx', rtol, atol):
      print "Output Matches!"
    else:
      print "Output Mismatch!"
    
#    if len(sys.argv) == 5:
#        rtol = float(sys.argv[3])
#        atol = float(sys.argv[4])

    
