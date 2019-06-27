import numpy as np
from astropy.io import fits
import sys
import os

#Input file information
inputPrefix = str(sys.argv[1])
numberInputFiles = int(sys.argv[2])
inputFiles = [inputPrefix + str(i) + '.npy' for i in range(numberInputFiles)]

#Reading input parameters from command line
outFile = str(sys.argv[3]) #Output FITS files

#Checking to see if file already exists
existBool = os.path.isfile(outFile)

a = [1 for i in range(100)]
b = [2 for i in range(100)]
c = [3 for i in range(100)]

#If the file doesn't exist
if not existBool:
	aArray = [a]
	bArray = [b]
	cArray = [c]
	col1 = fits.Column(name = 'a', format = '100E', unit = 'n/a', array = aArray)
	col2 = fits.Column(name = 'b', format = '100E', unit = 'n/a', array = bArray)
	col3 = fits.Column(name = 'c', format = '100E', unit = 'n/a', array = cArray)
	cols = fits.ColDefs([col1,col2,col3])
	hdu = fits.BinTableHDU.from_columns(cols)
	hdu.writeto(outFile)
	del hdu

	fitsData = fits.open(outFile)

	for file in inputFiles[1:]:
		aArray = [a]
		bArray = [b]
		cArray = [c]
		col1 = fits.Column(name = 'a', format = '100E', unit = 'n/a', array = aArray)
		col2 = fits.Column(name = 'b', format = '100E', unit = 'n/a', array = bArray)
		col3 = fits.Column(name = 'c', format = '100E', unit = 'n/a', array = cArray)
		cols = fits.ColDefs([col1,col2,col3])
		fitsData.append(fits.BinTableHDU.from_columns(cols))

	fitsData.writeto(outFile,overwrite=True)

else:
	fitsData = fits.open(outFile)
	for i in range(1,len(inputFiles)+1):
		currentHDU = fitsData[i].data
		aArray = [currentHDU[j][0] for j in range(len(currentHDU))]#.append(np.array(a,dtype='float32'))
		bArray = [currentHDU[j][1] for j in range(len(currentHDU))]#.append(np.array(b,dtype='float32'))
		cArray = [currentHDU[j][2] for j in range(len(currentHDU))]#.append(np.array(c,dtype='float32'))
		aArray.append(np.array(b,dtype='float32'))
		bArray.append(np.array(c,dtype='float32'))
		cArray.append(np.array(a,dtype='float32'))
		col1 = fits.Column(name = 'a', format = '100E', unit = 'n/a', array = aArray)
		col2 = fits.Column(name = 'b', format = '100E', unit = 'n/a', array = bArray)
		col3 = fits.Column(name = 'c', format = '100E', unit = 'n/a', array = cArray)
		cols = fits.ColDefs([col1,col2,col3])
		currentHDU = fits.BinTableHDU.from_columns(cols)
		fitsData[i] = currentHDU
	fitsData.writeto(outFile,overwrite=True)
