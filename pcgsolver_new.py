#!/usr/bin/python

import numpy as np
from scipy.sparse import *
from scipy import *
import sys
import pdb
from scipy.io import mmread, mmwrite

def generatePreconditioner(A, hw):
    P = np.zeros( (3, 2 * hw), dtype='float32' )
    a = A[1,0:hw]
    b = A[0,0:hw]
    c = A[2,hw::]
    d = A[1,hw::]

    det = (a * d) - (c * b)
    if(sum(det < 0) > 0):
        print("Error - non-pd block!")

    scale_diag = 1 / det
    scale_offdiag = -1 / det
    scale_diag[det <= 1e-4] = 1 
    scale_offdiag[det <= 1e-4] = 0

# add epsilon here

    P[1,:] = 1;
    P[0,0:hw] = scale_offdiag * b 
    P[1,0:hw] = scale_diag * d
    P[1,hw::] = scale_diag * a
    P[2,hw::] = scale_offdiag * c
    P_offset = np.array([-hw, 0, hw])

    return dia_matrix( (P, np.array([-hw, 0, hw])), shape=(2*hw,2*hw))


def pcg(D, x, b, P, maxIters=15):
    r = b - D*x
    z = P*r
    p = z.copy()
    cx = x
    res = b - D*cx
    print("Residual = ", dot(res,res))

    for i in xrange(maxIters):
	Ap = D*p
	v = dot(r, z)
	#print("abs(v): ", np.fabs(v))
        if(np.fabs(v) < 1e-5):
            break
        alpha = v/dot(p, Ap)
	cx = alpha*p + cx
	r = -alpha*Ap + r
        z = P*r
        beta = dot(z, r)/v
	p = beta*p + z
        
        res = b - D*cx;
        print("Residual = ", dot(res,res))

    return cx

def outputMatrixandVector(mat, vec):
  with open('matrix.txt', 'wb') as f:
    width, height = mat.shape
    for i in xrange(width):
      for j in xrange(height):
        value = mat.item((i,j))
        if value != 0.0:
          f.write(str(i) + ' ' + str(j) + ' ' + str(value) + '\n')
  
  with open('vector.txt', 'wb') as f:
    height = vec.size
    for i in xrange(height):
      f.write(vec[height] + '\n')
      
if __name__ == "__main__":
    
    print("This is a package")
    print("Running tests...")
    if(len(sys.argv) < 1):
        print("Data file not given")
        exit()

    f = open("Urban331.dat", "rb")
    width = np.fromfile(f, np.int32, 1)[0]
    height = np.fromfile(f, np.int32, 1)[0]
    
    A = np.fromfile(f, np.float32, width*height*12)
    b = np.fromfile(f, np.float32, width*height*2)
    
    Ar = np.reshape(A, (6,2*height*width))
    D_data = np.zeros( (7,2*height*width), dtype='float32' )

    D_data[1,0:width*height] = Ar[0,0:width*height]
    D_data[0,0:width*height] = Ar[0,width*height::]
    D_data[2,width*height::] = Ar[1,0:width*height]
    D_data[1,width*height::] = Ar[1,width*height::]
    D_data[3,0:-width] = Ar[2,width::]
    D_data[4,0:-1] = Ar[3,1::]
    D_data[5,1::] = Ar[4,0:-1]
    D_data[6,width::] = Ar[5,0:-width]
    D_offset = np.array([-width*height, 0, width*height, -width, -1, 1, width])
    D = dia_matrix( (D_data, D_offset), shape=(2*width*height,2*width*height))
    f.close()
    
#    np.savez('test', D=D,b=b,width=width,height=height)
#    foo = np.load('test.npz')
#    D = foo['D']
#    b = foo['b']
#    width = int32(foo['width'])
#    height = int32(foo['height'])
#    import code; code.interact(local=locals())

    import time
    start = time.time()

    b = np.reshape(b, (D.shape[0],), np.float32)
    x = np.zeros(b.shape, np.float32)
    P = generatePreconditioner(D_data, height*width)
    
    print D.shape
    print x.shape
    print b.shape
    print P.shape
#    b2 = csr_matrix(b)
#    mmwrite('matrix.txt', D)
#    mmwrite('vector.txt', b2)
    y = pcg(D,x,b,P)
    elapsed = time.time() - start
    print elapsed


