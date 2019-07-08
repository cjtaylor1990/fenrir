import numpy as np
import matplotlib.pyplot as plt
import csv
import sys

def read_txt_file(inFile,numVals = 1,delimiter = ' ',isFloat=True):
	"""This function reads in the given text file and gives its contents back as a list
	of lists, each sublist a different column of the original text file.

	Example:
	If example.txt is:
	1 2
	3 4

	Then,
	read_txt_file('example.txt',numVals = 2, delimiter = ' ', isFloat=True)
	returns
	[[1,3],[2,4]]

	numVals = number of columns (default 1)
	delimiter = delimiter of columns (default ' ')
	isFloat = tells function if you want it in float (True)
		or string (False) format (default True)
	"""
	table = open(inFile, 'r')
	data = csv.reader(table, delimiter = delimiter)

	outList = []
	i = 0
	while (i < numVals):
		outList.append([])
		i+=1

	for row in data:
		i = 0
		while (i < numVals):
			if isFloat==True:
				item = float(row[i])
			else:
				pass
			outList[i].append(item)
			i+=1

	return outList

#Input information
inPath = '/Users/cjtaylor/fenrir_full_tests/'
inAfile  = inPath + 'full_spin_list.txt'
inMuFile = inPath + 'full_mu_list.txt'

#Output information
outPath = '/Users/cjtaylor/fenrir_full_tests/'
machineNames = ['yorp1','yorp2','yorp4','yorp6','yorp7','yorp8','yorp9']
outSuffix = '_full_parameter_list.txt'

#Number of combinations per machine
numComb = [130,130,130,90,90,90,90]

#Reading in parameter values from input files
aData = read_txt_file(inAfile)[0]
muData = read_txt_file(inMuFile)[0]

#Checking to make sure there are no errors in input
if (sum(numComb) != len(aData)*len(muData)):
	errorMessage="""ERROR: The number of parameter combinations given to the machines does not match
	the total number of possible combinations from the input lists."""
	sys.exit(errorMessage)
elif (len(machineNames) != len(numComb)):
	errorMessage="""ERROR: The number of machines does not match the number of parameter combinations
	given."""
	sys.exit(errorMessage)
else:
	pass

#Creating list of parameter combinations
combData = []
i = 0
while (i < len(aData)):
	j = 0
	aVal = aData[j]
	while (j < len(muData)):
		combData.append([aData[i],muData[j]])
		j+=1
	i+=1

i = 0 #Index loops over machines
k = 0 #Index loops over the parameter combination list
while (i < len(machineNames)):
	machine = machineNames[i]
	machineComb = numComb[i]
	outFile = machine + outSuffix

	file = open(outFile,'w')
	j = 0 #Index counts to make sure each machine is given correct number of combinations
	while (j < machineComb):
		file.write(str(combData[k][0]) + ' ' + str(combData[k][1]) + '\n')
		j+=1
		k+=1
	file.close()
	print outFile + ' finished' #Printing confirmation statement
	i+=1
