import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

"""
This is a code that is to load in the paramater list of the test grid, turn that parameter combination lists
into seperate FITS table objects. It will then load in the individual FITS files given to the program via a .txt file
and then combine those new table objects with the transfer function tables already
calculated in said FITS files: [header,spin,mu,table1,table2,...]. It should then rename each of the transfer function tables
by a pair of dummy indices (a.index_mu.index) and combine everything into one large FITS file.

Last updated: Corbin Taylor (12:16 pm, 2/26/19)
"""

#Defining relevant file paths
fileDirectory = str(sys.argv[1])
listFitsFile = fileDirectory + str(sys.argv[2])
outFitsFile = fileDirectory + str(sys.argv[3])
paramFile = fileDirectory + str(sys.argv[4])

#Creating list CSV reader to read in file paths from list .txt file
list = open(listFitsFile, 'r')
listReader = csv.reader(list, delimiter = ' ')

#Looping over list file with CSV reader, creating the list of file paths
inFitsFiles = []
for entry in listReader:
	inFitsFiles.append(fileDirectory + str(entry[0]))
del list,listReader
print(inFitsFiles)
#Section 1: Loading in paramater txt file data
table = open(paramFile, 'r')
data = csv.reader(table, delimiter = ' ')

a = []
mu = []

for column in data:
	aVal = float(column[0])
	muVal = float(column[1])
	a.append(aVal)
	mu.append(muVal)
a = np.unique(np.array(a))
mu = np.unique(np.array(mu))

del table,data

#Converting npy arrays into a new FITS file
aHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'a', format = '1E', unit = 'M', array = a)],name='a')
muHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'mu0', format = '1E', array = mu)],name='mu0')
aHDU.writeto(outFitsFile)
fitsData = fits.open(outFitsFile)
fitsData.append(muHDU)
fitsData.writeto(outFitsFile, overwrite=True)
fitsData.close()

del fitsData

#Creating placeholder angle array
angleArray = np.zeros(20)+1.

#Creating new HDU columns for placeholder angle array
cols = []
cols.append(fits.Column(name = 'cosne1', format = '20E', unit = 'k=1', array = angleArray))
cols.append(fits.Column(name = 'cosne2', format = '20E', unit = 'k=2', array = angleArray))
newCols = fits.ColDefs(cols)

#Setting up dummy variable to loop over all of the individual FITS files
i = 0

#Dummy variables to loop over a and mu = cos(i)
j = 1
k = 1

#Opening FITS outfile
fitsData = fits.open(outFitsFile)

#Loading in data from the individual i-th FITS file
print inFitsFiles[i]
data = fits.open(inFitsFiles[i])
	
#Defining dummy variable to loop over the tables of the FITS file
m = 1

#Starting nested loops over a and mu values (j,k respectively)
while (j <= len(a)):
	while(k <= len(mu)):
		#Checking to see if we've reached the end of the i-th FITS file
		if (m == len(data)):
			print "Switching Files!"
			#Emptying variable to be re-used
			del data
			#Increasing the FITS file dummy variable by 1
			i+=1
			#Loading in new FITS file
			data = fits.open(inFitsFiles[i])
			#Setting the table variable back to 1
			m = 1
			print inFitsFiles[i]

		#Getting data and columns of m-th table of i-th FITS file
		oldTranHDU = data[m]
		oldCols = oldTranHDU.columns
			
		#Combining m-th table with new angle data
		newTranHDU = fits.BinTableHDU.from_columns(oldCols + newCols)
			
		#Renaming the HDU object by the RELXILL standard (a.index_i.index = j_k)
		newTranHDU.name = str(j) + '_' + str(k)
		fitsData.append(newTranHDU)
			
		#Advancing the mu dummy variable by one
		k+=1
		#Advancing the table dummy variable by one
		m+=1
	#Setting the mu dummy variable back to one once it has looped over the mu values
	k=1
	j+=1
fitsData.writeto(outFitsFile, overwrite=True)
fitsData.close()
data.close()