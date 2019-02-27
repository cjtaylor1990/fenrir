import numpy as np
import matplotlib.pyplot as plt
import csv
import sys

inFile  = str(sys.argv[1])
outFile = str(sys.argv[2])

table = open(inFile, 'r')
data = csv.reader(table, delimiter = ' ')

file = open(outFile,'w')
for row in data:
	a = float(row[0])
	i = np.degrees(np.arccos(float(row[1])))
	file.write(str(a) + ' ' + str(i) + '\n')
file.close()
