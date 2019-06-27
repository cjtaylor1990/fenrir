import numpy as np
from astropy.io import fits
import sys
import os

#Reading input parameters from command line
outFile = str(sys.argv[1]) #Output FITS files

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
else:
	fitsData = fits.open(outFile)[1].data
	print(fitsData[0][1])
	print(list(range(len(fitsData))))
	aArray = [fitsData[i][0] for i in range(len(fitsData))]#.append(np.array(a,dtype='float32'))
	bArray = [fitsData[i][1] for i in range(len(fitsData))]#.append(np.array(b,dtype='float32'))
	cArray = [fitsData[i][2] for i in range(len(fitsData))]#.append(np.array(c,dtype='float32'))
	aArray.append(np.array(b,dtype='float32'))
	bArray.append(np.array(c,dtype='float32'))
	cArray.append(np.array(a,dtype='float32'))
	col1 = fits.Column(name = 'a', format = '100E', unit = 'n/a', array = aArray)
	col2 = fits.Column(name = 'b', format = '100E', unit = 'n/a', array = bArray)
	col3 = fits.Column(name = 'c', format = '100E', unit = 'n/a', array = cArray)
	cols = fits.ColDefs([col1,col2,col3])
	hdu = fits.BinTableHDU.from_columns(cols)
	hdu.writeto(outFile,overwrite=True)
