import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

"""
This program is to take in a list of FITS table files and combine them (in the order given to them) as a unified table.
"""

#Defining relevant file paths
fileDirectory = str(sys.argv[1])
listFile = fileDirectory + "fits_to_combine.txt"
outFile = str(sys.argv[2])

#Creating list CSV reader to read in file paths from list .txt file
list = open(listFile, 'r')
listReader = csv.reader(list, delimiter = ' ')

#Looping over list file with CSV reader, creating the list of file paths
inFitsFiles = []
for entry in listRader:
	inFitsFiles.append(fileDirectory + str(entry[0])) 

#Loading in first FITS file in list to which the others will subsequently be appended
firstEntry = inFitsFiles[0]
combinedData = fits.open(firstEntry)

for file in inFitsFiles[1:]:
	#Loading FITS file from file path entry
	fitsData = fits.open(file)
	
	#Appending HDUTable object to FITS data
	combinedData.append(fitsData)
	
	#Closing FITS file
	fitsData.close()

#Saving combined FITS file
combinedData.writeto(outFile, overwrite=True)