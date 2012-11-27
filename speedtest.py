import sys
import csv
import os

def readtemp(filename):
  f = open(filename, 'rb')
  for line in f:
    if line.find("Time to solve:") != -1:
      time = int(line[15:17])
      break;
  return time

if __name__ == "__main__":
  numcores = int(sys.argv[1])
  print "Numcores: " + str(numcores)
  
  legend = ['numcores']
  i=1
  while i <= numcores:
    legend.append(str(i))
    i *= 2
    
  print legend
  
  objlist = []
  newlist = []
  lllist = []
  newlllist = []
  
  print "Running Tests for solver with objects: "
  objlist.append('object-solver')
  os.chdir('cpp')
  for i in legend[1:]:
    filename = i+'.txt'
    i = int(i)
    os.system("./driver " + str(i) + " 1 >> " + filename)
    t = str(readtemp(filename))
    os.system("rm " + filename)
    objlist.append(t)
    print str(i) + " cores: " + t
  os.chdir('..')
  
  print "Running Tests for newsolver (uses map): "
  newlist.append('newsolver')
  os.chdir('newcpp')
  for i in legend[1:]:
    filename = i+'.txt'
    i = int(i)
    os.system("./newsolver " + str(i) + " 1 >> " + filename)
    t = str(readtemp(filename))
    os.system("rm " + filename)
    newlist.append(t)
    print str(i) + " cores: " + t
  os.chdir('..')
    
  print "Running Tests for llsolver with Linked Lists: "
  lllist.append('llsolver')
  os.chdir('llcpp')
  for i in legend[1:]:
    filename = i+'.txt'
    i = int(i)
    os.system("./llsolver "  + str(i) + " 1 >> " + filename)
    t = str(readtemp(filename))
    os.system("rm " + filename)
    lllist.append(t)
    print str(i) + " cores: " + t
  os.chdir('..')
  
  print "Running Tests for llsolver with Optimized Linked Lists: "
  newlllist.append('opt-llsolver')
  os.chdir('llcpp-optimized')
  for i in legend[1:]:
    filename = i+'.txt'
    i = int(i)
    os.system("./llsolver " + str(i) + " 1 >> " + filename)
    t = str(readtemp(filename))
    os.system("rm " + filename)
    newlllist.append(t)
    print str(i) + " cores: " + t
  os.chdir('..')
  
  with open('speedtest.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    spamwriter.writerow(legend)
    spamwriter.writerow(objlist)
    spamwriter.writerow(newlist)
    spamwriter.writerow(lllist)
    spamwriter.writerow(newlllist)
  
  print "Finished Writing to File!"
  
  