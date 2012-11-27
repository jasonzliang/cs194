import sys
import csv
import os

def readtemp():
  f = open('temp.txt', 'rb')
  for line in f:
    if line.find("Time to solve:") != -1:
      time = int(line[15:17])
      break;
  return time

if __name__ == "__main__":
  numcores = int(sys.argv[1])
  print "Numcores: " + str(numcores)
  
  legend = ['numcores']
  for i in xrange(numcores):
    legend.append(str(i+1))
    
  newlist = []
  lllist = []
  newlllist = []
  
  print "Running Tests for newsolver (uses map): "
  newlist.append('newsolver')
  os.chdir('newcpp')
  for i in xrange(numcores):
    os.system("./newsolver " + str(i+1) + " 1 >> temp.txt")
    t = readtemp()
    os.system("rm temp.txt")
    newlist.append(t)
    print str(i+1) + " cores: " + str(t)
  os.chdir('..')
    
  print "Running Tests for llsolver with Linked Lists: "
  lllist.append('llsolver')
  os.chdir('llcpp')
  for i in xrange(numcores):
    os.system("./llsolver " + str(i+1) + " 1 >> temp.txt")
    t = readtemp()
    os.system("rm temp.txt")
    lllist.append(t)
    print str(i+1) + " cores: " + str(t)
  os.chdir('..')
  
  print "Running Tests for llsolver with Optimized Linked Lists: "
  newlllist.append('opt-llsolver')
  os.chdir('llcpp-optimized')
  for i in xrange(numcores):
    os.system("./llsolver " + str(i+1) + " 1 >> temp.txt")
    t = readtemp()
    os.system("rm temp.txt")
    newlllist.append(t)
    print str(i+1) + " cores: " + str(t)
  os.chdir('..')
  
  with open('speedtest.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    spamwriter.writerow(legend)
    spamwriter.writerow(newlist)
    spamwriter.writerow(lllist)
    spamwriter.writerow(newlllist)
  
  print "Finished Writing to File!"
  
  