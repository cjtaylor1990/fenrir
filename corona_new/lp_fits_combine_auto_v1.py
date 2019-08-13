import numpy as np
from astropy.io import fits
import sys
import os
import csv

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

print(sys.argv)
#Input from command line
filePath = str(sys.argv[1]) #File path to directory you wish to store files
inputPrefix = str(sys.argv[2]) #Prefix of .fits files that you're loading (used for auto generating file list)
numberHeights = int(sys.argv[3]) #Number of heights used
initThickness = float(sys.argv[4]) #Starting (max) thickness
finalThickness = float(sys.argv[5]) #Final (min) thickness
numberThickness = int(sys.argv[6]) #Number of thicknesses used
paramFile = str(sys.argv[7]) #File that stores spin values (.txt)
outFile = filePath + str(sys.argv[8]) #Output FITS files

if os.path.isfile(outFile): #Checking to see if file already exists
	print("ERROR! File already exits!")
	sys.exit()

#Auto generating list of input .fits files
inputFiles = [filePath + inputPrefix + str(i) + '.fits' for i in range(1,numberHeights+1)]

#Reading in spin and heights from parameter file
paramArray = np.array(read_txt_file(paramFile, numVals = 2))

#Parsing the parameter array into two seperate arrays, one just for spin values
#and the other being a two dimensional array of heights (index corresponds to spin)
spinArray = np.unique(paramArray[0]) #Finding unique spin values
heightArray = np.zeros((len(spinArray),numberHeights)) #Alocating array for heights
#For each spin value...
for i in range(len(spinArray)):
	#Set the corresponding index in the height array equal to the subarray of
	#height values that correspond with the spin value
	heightArray[i] = paramArray[1,i*numberHeights : (i+1)*numberHeights] 

#Creating the FITS file by first writing thickness HDU
if numberThickness == 1:
	thicknessArray = np.array([initThickness])
else:
	thicknessArray = np.array([initThickness + (finalThickness-initThickness)*i/(numberThickness-1) for i in range(numberThickness)])
thicknessHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'hd', format = '1E', array = thicknessArray)],name='hd')
thicknessHDU.writeto(outFile)

#For each thickness
for i in range(1,numberThickness+1):
	fluxColumnList = []
	delColumnList = []
	delIncColumnList = []
	for file in inputFiles:
		currentHDU = fits.open(file)
		currentHDU = currentHDU[i].data
		#radColumn = [row[0] for row in currentHDU]
		fluxColumn = [[row[1] for row in currentHDU]]
		delColumn = [[row[2] for row in currentHDU]]
		delIncColumn = [[row[3] for row in currentHDU]]
		fluxColumnList += fluxColumn
		delColumnList += delColumn
		delIncColumnList += delIncColumn


	combinedSpin = [fits.Column(name='a',format='1E',unit='', array = list(spinArray))]
	combinedRadius = [fits.Column(name='r',format='150E',unit='GM/c^2', array = [row[0] for row in currentHDU])]
	combinedHeight = [fits.Column(name='hgrid', format='{}E'.format(numberHeights), unit='GM/c^2', array = list(heightArray))]
	combinedFlux = [fits.Column(name='h{}'.format(k+1),format='150E', unit = 'GM/c^2', array = fluxColumnList[k]) for k in range(numberHeights)]
	combinedDel = [fits.Column(name='del{}'.format(k+1),format='150E', unit = 'deg', array = delColumnList[k]) for k in range(numberHeights)]
	combinedDelInc = [fits.Column(name='del_inc{}'.format(k+1),format='150E', unit = 'deg', array = delIncColumnList[k]) for k in range(numberHeights)]
	cols = fits.ColDefs(combinedSpin + combinedHeight + combinedRadius + combinedFlux + combinedDel + combinedDelInc) #appending the column lists together
	hdu = fits.BinTableHDU.from_columns(cols)
	hdu.name = "l_h_" + str(i)

	fitsData = fits.open(outFile)
	fitsData.append(hdu)
	fitsData.writeto(outFile,overwrite=True)
