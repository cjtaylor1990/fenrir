import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

#python2 create_params.py 1 0 0 5 15 35 $param_file
aNumCases = int(sys.argv[1])
aMin = float(sys.argv[2])
aMax = float(sys.argv[3])
iNumCases = int(sys.argv[4])
iMin = float(sys.argv[5])
iMax = float(sys.argv[6])
outFile = str(sys.argv[7])

aArray = np.linspace(aMin,aMax,aNumCases)
iArray = np.linspace(iMin,iMax,iNumCases)

file = open(outFile,'w')

i = 0
while (i < aNumCases):
	aVal = aArray[i]
	j = 0
	while (j < iNumCases):
		iVal = iArray[j]
		file.write(str(aVal)+' '+str(iVal) +'\n')
		j+=1
	i+=1

file.close()