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
inParamFile = input("What is your full input parameter file? ")
paramData = read_txt_file(inParamFile, numVals=2)

#Output information
outPath = input("Where do you want your output files to be saved? ")
outSuffix = '_' + input("What will be the suffix of your output files (format: [machine name]_[suffix].txt)? ") + '.txt'

#Machine information
machineNames = str(input("What machines will you be using, seperating names by spaces? ")).lower()
machineNames = machineNames.split(' ')

#Splitting jobs across machines
isValid = False
while not isValid:
	behavior = input("""Do you want the jobs split evenly across each machine (default) or
	would you like to split them yourself (custom)?  """).lower()
	
	if behavior == 'default':
		numComb = [len(paramData[0])//len(machineNames) for i in range(len(machineNames))]
		isValid = True
	elif behavior == 'custom':
		for i in range(len(machineNames)):
			numComb.append(int(input("How many parameter combinations for {}? ".format(machineNames[i]))))
		isValid = True
	else:
		print("Please enter valid input of either 'default' or 'custom'.\n")

print(numComb)

#Checking to make sure there are no errors in input
if (sum(numComb) != len(paramData[0])):
	errorMessage="""ERROR: The number of parameter combinations given to the machines does not match
	the total number of possible combinations from the input lists."""
	sys.exit(errorMessage)
else:
	pass

k = 0 #Points to entry in full parameter combination list
for i in range(len(machineNames)):
	machine = machineNames[i] #Points to current machine
	machineComb = numComb[i] #Points towards number of combinations that will be given to said machine
	outFile = outPath + machine + outSuffix #Creating the full output file path

	#Opening output file
	file = open(outFile,'w')

	#Looping through the number of parameter combination spectified for the current machine
	for j in range(machineComb): 
		#Writing current parameter combination to end of current output file
		file.write(str(paramData[0][k]) + ' ' + str(paramData[1][k]) + '\n')
		
		#Moving to next entry in full list
		k+=1
	#After looping through the specified number of combinations, close the current output file
	file.close()
	print(outFile + ' finished') #Printing confirmation statement
