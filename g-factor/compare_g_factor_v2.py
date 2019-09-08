import numpy as np
import matplotlib.pyplot as pl
import sys

#Taking in input parameters (paths to input files and their individual names)
filePath = sys.argv[1]
file1 = filePath + sys.argv[2]
file2 = filePath + sys.argv[3]
file3 = filePath + sys.argv[4]

#Opening each file
data1 = np.load(file1)
data2 = np.load(file2)
data3 = np.load(file3)

#Overplotting each of them
pl.plot(data1[1], data1[0], c='k')
pl.scatter(data2[1], data2[0], c='r')
pl.plot(data3[1], data3[0], c='b')
pl.xscale('log')
pl.yscale('log')
pl.show()