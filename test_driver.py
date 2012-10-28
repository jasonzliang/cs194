"""
Compares output of both solvers.

Assumes input of this format - one float on each line:
0.3343
0.343
0.34343
..

"""
import sys

def compare_output(file1, file2, threshold=0.0001):
    f1 = open(file1,'r')
    f2 = open(file2, 'r')

    try:
        while True:
            l1 = f1.readline().replace("\n","")
            l2 = f2.readline().replace("\n","")

            if l1 == "" or l2 == "":
                break
            
            diff = abs(float(l1) - float(l2))
            if diff > threshold:
                print "Mismatch: %s, %s; Differ by %f" % (l1, l2, diff)

    except Exception as e:
        print e
        f1.close()
        f2.close()
    finally:
        f1.close()
        f2.close()

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
    
