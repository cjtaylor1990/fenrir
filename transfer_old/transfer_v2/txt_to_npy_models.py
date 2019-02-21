import numpy as np
import matplotlib.pyplot as plt
import csv
import sys

inFile  = str(sys.argv[1])
outFile = str(sys.argv[2])

table = open(inFile, 'r')
data = csv.reader(table, delimiter = ' ')

energy = []
numFlux = []

for column in data:
	energy.append(float(column[0]))
	numFlux.append(float(column[2]))

energy = np.array(energy)
numFlux = np.array(numFlux)

outArray = np.array([energy,numFlux])

np.save(outFile, outArray)
