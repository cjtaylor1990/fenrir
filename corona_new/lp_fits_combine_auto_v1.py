import numpy as np
from astropy.io import fits
import sys
import os

#Input file information
filePath = str(sys.argv[1])
inputPrefix = str(sys.argv[2])
numberHeights = int(sys.argv[3])
numberThickness = int(sys.argv[4])
inputFiles = [filePath + inputPrefix + str(i) + '.fits' for i in range(1,numberHeights+1)]

#Reading input parameters from command line
outFile = filePath + str(sys.argv[5]) #Output FITS files

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

	#print(len(radColumn),len(radColumn[0]))
	combinedRadius = [fits.Column(name='r',format='150E',unit='GM/c^2', array = [row[0] for row in currentHDU])]
	combinedFlux = [fits.Column(name='h{}'.format(k+1),format='150E', unit = 'GM/c^2', array = fluxColumnList[k]) for k in range(numberHeights)]
	combinedDel = [fits.Column(name='del{}'.format(k+1),format='150E', unit = 'deg', array = delColumnList[k]) for k in range(numberHeights)]
	combinedDelInc = [fits.Column(name='del_inc{}'.format(k+1),format='150E', unit = 'deg', array = delIncColumnList[k]) for k in range(numberHeights)]
	cols = fits.ColDefs(combinedRadius + combinedFlux + combinedDel + combinedDelInc) #appending the column lists together
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
