import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

def rEvent(spin):
	return 1+np.sqrt(1-spin**2)

#Getting input/output file paths from the user
filePath = sys.argv[1]
spinListFile = filePath + sys.argv[2]
outFile = filePath + sys.argv[3]

#Getting number of cases and the max height value from the user (minimum h is assumed to be 1.1*rEvent(a))
hNumCases = int(sys.argv[4])
hMax = float(sys.argv[5])

#Loading in spin values from spin file
spinVals = csv.reader(open(spinListFile),delimiter = ' ')

#Opening file to output file
output = open(outFile,'w')

#For each spin value
for spin in spinVals:
	rEventVal = rEvent(float(spin[0]))
	hMin = 1.1*rEventVal
	#For each h value
	for j in range(hNumCases):
		#Calculating h value (linear in log10 space)
		hVal = np.log10(hMin) + j*((np.log10(hMax)-np.log10(hMin))/(hNumCases-1))
		hVal = 10.**hVal
		#Writing to output file (spin height)
		output.write(spin[0]+' '+str(hVal) +'\n')

#Closing output file
output.close()
