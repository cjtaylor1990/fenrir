import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os
import my_utilities as myutil

"""
Need to write description of code

Last updated: Corbin Taylor (11:53 am, 8/13/19)
"""

#Defining relevant file paths
fileDirectory = str(sys.argv[1])
listFitsFile = fileDirectory + str(sys.argv[2])
outFitsFile = fileDirectory + str(sys.argv[3])
#paramFile = fileDirectory + str(sys.argv[4])
#numberHeights = int(sys.argv[5])

#Reading in list of FITS files from listFitsFile
listFits = myutil.read_txt_file(listFitsFile, isFloat=False)[0]

#Reading in first FITS file and grabbing the disk thicknesses and HDU columns
hdData = fits.open(listFits[0])
hd = list(hdData[1].data['hd'])
cols = [col.name for col in hdData[2].data.columns]
formats = [col.format for col in hdData[2].data.columns]
units = [col.unit for col in hdData[2].data.columns]
del hdData

#Finding number of thicknesses and columns
numThickness = len(hd)
numCols = len(cols)

#Putting that information in new FITS file that will be the combined file
hdHDU = fits.BinTableHDU.from_columns([fits.Column(name = 'hd', format = '1E', unit = 'RG', array = hd)],name='hd')
hdHDU.writeto(outFitsFile)

combinedData = fits.open(outFitsFile)

for i in range(2,numThickness+2):
	#Creating storage for current combined HDU data
	colsForHDU = [[] for j in range(numCols)]
	for j in range(len(listFits)):
		currentData = fits.open(listFits[j])[i].data
		for k in range(numCols):
			colsForHDU[k] += [row[k] for row in currentData]
	
	for j in range(numCols):
		colsForHDU[j] = fits.Column(name=cols[j],format=formats[j],unit=units[j],array=colsForHDU[j])

	colsForHDU = fits.ColDefs(colsForHDU)
	
	hdu = fits.BinTableHDU.from_columns(colsForHDU)
	hdu.name = "l_h_" + str(i-1)

	combinedData.append(hdu)

combinedData.writeto(outFitsFile,overwrite=True)