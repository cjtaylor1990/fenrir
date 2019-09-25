import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

"""
New description here.
"""

#Defining relevant file paths
fileDirectory = str(sys.argv[1])
listFitsFile = fileDirectory + str(sys.argv[2])
outFitsFile = fileDirectory + str(sys.argv[3])
paramFile = fileDirectory + str(sys.argv[4])
indivParamListFile = fileDirectory + str(sys.argv[5])

#Creating list CSV reader to read in file paths from list .txt file
filePaths = open(listFitsFile, 'r')
listReader = csv.reader(filePaths, delimiter = ' ')

#Looping over list file with CSV reader, creating the list of file paths
inFitsFiles = []
for entry in listReader:
	inFitsFiles.append(fileDirectory + str(entry[0]))
del filePaths,listReader
print(inFitsFiles)

#Opening list of individual param files
filePaths = open(indivParamListFile, 'r')
print(filePaths)
listReader = csv.reader(filePaths, delimiter = ' ')


#Looping over list file with CSV reader, creating the list of param files
indivParamNumbers = []
for entry in listReader:
	print(entry)
	paramList = open(entry[0], 'r')
	print(paramList)
	paramListReader = csv.reader(paramList, delimiter = ' ')
	count = 0
	for param in paramListReader:
		count+=1
	indivParamNumbers.append(count)
	del paramList, paramListReader

del filePaths,listReader
print(indivParamNumbers)

#Section 1: Loading in paramater txt file data
table = open(paramFile, 'r')
data = csv.reader(table, delimiter = ' ')

a = []
mu = []
hd = []

for column in data:
	aVal = float(column[0])
	muVal = float(column[1])
	hdVal = float(column[2])
	a.append(aVal)
	mu.append(muVal)
	hd.append(hdVal)
a = np.unique(np.array(a))
mu = np.unique(np.array(mu))
hd = np.unique(np.array(hd))[::-1]

del table,data

#Converting npy arrays into a new FITS file
aHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'a', format = '1E', unit = 'M', array = a)],name='a')
muHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'mu0', format = '1E', array = mu)],name='mu0')
hdHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'hd', format = '1E', unit = 'RG', array = hd)],name='hd')
aHDU.writeto(outFitsFile)
fitsData = fits.open(outFitsFile)
fitsData.append(muHDU)
fitsData.append(hdHDU)
fitsData.writeto(outFitsFile, overwrite=True)
fitsData.close()

del fitsData

#Creating placeholder angle array
angleArray = np.zeros((100,20))+1.
print(angleArray)

#Creating new HDU columns for placeholder angle array
cols = []
cols.append(fits.Column(name = 'cosne1', format = '20E', unit = 'k=1', array = angleArray))
cols.append(fits.Column(name = 'cosne2', format = '20E', unit = 'k=2', array = angleArray))
newCols = fits.ColDefs(cols)

#Setting up dummy variable to loop over all of the individual FITS files
i = 0

#Dummy variables to loop over a, mu = cos(i), and hd
j = 1 #a dummy variable
k = 1 #mu dummy variable
h = 1 #hd dummy variable

#Opening FITS outfile
fitsData = fits.open(outFitsFile)

#Setting up dummy to scan through list of FITS file lengths
lengthPointer = 0

#Finding current FITS length
currentLength = indivParamNumbers[lengthPointer]

#Creating list of input files
currentFiles = []
fileIndex = 0
while (fileIndex < len(hd)):
	currentFiles.append(inFitsFiles[i])
	fileIndex+=1
	i+=1
#currentFiles.reverse()

#Defining dummy variable to loop over the tables of the FITS file
m = 1

#Starting nested loops over a, mu, hd values (j,k,h respectively)
while (j <= len(a)):
	while(k <= len(mu)):
		while (h <= len(hd)):
			
			#Loading FITS file
			data = fits.open(currentFiles[h-1])

			#Getting data and columns of m-th table of i-th FITS file
			oldTranHDU = data[m]
			oldCols = oldTranHDU.columns
			
			#Combining m-th table with new angle data
			newTranHDU = fits.BinTableHDU.from_columns(oldCols + newCols)
			
			#Renaming the HDU object by the RELXILL standard (a.index_i.index = j_k)
			newTranHDU.name = str(j) + '_' + str(k) + '_' + str(h)
			fitsData.append(newTranHDU)
			
			#Advancing the hd dummy variable by one
			h+=1
			
		print('Row: ' + str(m))
		#Checking to see if we've reached the end of the current set of FITS files
		if (m == currentLength) and (j < len(a)):
			print("Switching Files!")
				
			#Creating list of input files
			currentFiles = []
			fileIndex = 0
			while (fileIndex < len(hd)):
				currentFiles.append(inFitsFiles[i])
				fileIndex+=1
				i+=1
			#currentFiles.reverse()
			#Setting the table variable back to 1
			m = 1

			lengthPointer += 1
			currentLength = indivParamNumbers[lengthPointer]
		else:
			#Advancing the table dummy variable by one
			m+=1
		
		#Setting the hd dummy variable back to one once it has looped over the hd values
		h=1
		
		#Advancing the mu dummy variable by one
		k+=1

	#Setting the mu dummy variable back to one once it has looped over the mu values
	k=1
	#Advancing the a dummy variable by one
	j+=1

fitsData.writeto(outFitsFile, overwrite=True)
fitsData.close()