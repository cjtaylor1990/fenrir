import numpy as np
from astropy.io import fits
import sys
import os

#Input file information
inputPrefix = str(sys.argv[1])
numberHeights = int(sys.argv[2])
numberThickness = int(sys.argv[3])
inputFiles = [inputPrefix + '_' + str(i) + '.fits' for i in range(numberHeights)]

#Reading input parameters from command line
outFile = str(sys.argv[4]) #Output FITS files

for i in range(1,numberThickness+1):
	aList = []
	bList = []
	cList = []
	for file in inputFiles:
		#print(file)
		currentHDU = fits.open(file)
		#print(currentHDU)
		currentHDU = currentHDU[i].data
		aArray = [[row[0] for row in currentHDU]]
		bArray = [[row[1] for row in currentHDU]]
		cArray = [[row[2] for row in currentHDU]]
		aList += aArray
		bList += bArray
		cList += cArray
	print(aList[0])
	aColumnList = [fits.Column(name='a{}'.format(k+1),format='100E', unit = 'n/a', array = aList[k]) for k in range(numberHeights)]
	bColumnList = [fits.Column(name='b{}'.format(k+1),format='100E', unit = 'n/a', array = bList[k]) for k in range(numberHeights)]
	cColumnList = [fits.Column(name='c{}'.format(k+1),format='100E', unit = 'n/a', array = cList[k]) for k in range(numberHeights)]
	cols = fits.ColDefs(aColumnList + bColumnList + cColumnList) #appending the column lists together
	hdu = fits.BinTableHDU.from_columns(cols)

	if i == 1:
		if os.path.isfile(outFile): #Checking to see if file already exists
			print("ERROR! File already exits!")
			sys.exit()
		else:
			hdu.writeto(outFile)
	else:
		fitsData = fits.open(outFile)
		fitsData.append(hdu)
		fitsData.writeto(outFile,overwrite=True)
