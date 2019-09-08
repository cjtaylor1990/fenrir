import numpy as np
import csv
import sys

inFile  = str(sys.argv[1])
outFile = str(sys.argv[2])

table = open(inFile, 'r')
data = csv.reader(table, delimiter = ' ')

radius = []
gFactor = []

for row in data:
	radius.append(float(row[0]))
	gFactor.append(float(row[1]))

outArray = np.array([gFactor,radius])

np.save(outFile, outArray)
