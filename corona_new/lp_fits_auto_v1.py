import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

def rIsco(a):
	"""Calculates the inner-most stable circular orbit (or rIsco, in gravitational
	radii Rg) for a given dimensionless spin value. |a| <= 1

	Example: rIsco(0) => 6.0 """
	z1 = 1 + (((1-(a*a))**(1./3.))*(((1+a)**(1./3.))+((1-a)**(1./3.))))
	z2 = ((3*(a*a))+(z1*z1))**0.5
	if (a < 0) and (a >= -1):
		rOut = 3+z2+(((3-z1)*(3+z1+(2.*z2)))**0.5)
	elif (a >= 0) and (a <= 1):
		rOut = 3+z2-(((3-z1)*(3+z1+(2.*z2)))**0.5)
	else:
		print("ERROR: Spin parameter must be within range of -1 to 1, inclusive.")
		sys.exit()
	return rOut

#Reading input parameters from command line: spin, corona_height, output_file, number_of_layers, file1, file2, ...
a = float(sys.argv[1]) #Spin of black hole (dimensionless)
hCorona = float(sys.argv[2]) #Height of corona along polar axis (r_g)
outFile = str(sys.argv[3]) #Output FITS files
numLayers = int(sys.argv[4]) #Number of .npy input files (one for each image size)

#Array of emissivity .npy files that are given by command line
emissivityFileArray = np.array([str(file) for file in sys.argv[5:]])

#Checking to make sure emissivityFileArray has length = number_of_layer
if len(emissivityFileArray) != numLayers:
	print("ERROR: You must have same number of layers as input file. Exiting program...")
	sys.exit()

#Checking to see if file already exists
existBool = os.path.isfile(outFile)

#Looping over array of emissivity .npy files
for file in emissivityFileArray:
	data = np.load(file) #Loading in file
	emissivityData = data[0] #Extracting emissivity data
	radius = emissivityData[0] #Extracting radii values
	flux = emissivityData[1] #Extracting flux v. radii
	delta = emissivityData[2] #Extracting mean(delta) v. radii

	#If the file doesn't exist
	if not existBool:
		fluxArray = flux
		deltaArray = delta
		radiusColumn = fits.Column(name = 'r', format = '1E', unit = 'GM/c^2', array = radius)
		fluxColumn = fits.Column(name = 'h1', format = '1E', unit = 'GM/c^2', array = fluxArray)
		deltaColumn = fits.Column(name = 'del1', format = '1E', unit = 'deg', array = deltaArray)
		cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn])
		hdu = fits.BinTableHDU.from_columns(cols)
		hdu.writeto(outFile)
	else:
		fitsData = fits.open(outFile)[1].data
		print(fitsData[0][1])
		fluxArray = [fitsData[i][1] for i in range(len(fitsData))].append(np.array(radius,dtype='float32'))
		deltaArray = [fitsData[i][2] for i in range(len(fitsData))].append(np.array(delta,dtype='float32'))
		radiusColumn = fits.Column(name = 'r', format = '2E', unit = 'GM/c^2', array = fitsData[0])
		fluxColumn = fits.Column(name = 'h1', format = '2E', unit = 'GM/c^2', array = fluxArray)
		deltaColumn = fits.Column(name = 'del1', format = '2E', unit = 'deg', array = deltaArray)
		cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn])
		hdu = fits.BinTableHDU.from_columns(cols)
		hdu.writeto(outFile,overwrite=True)


	"""
	#Creating HDUTable
	hdu = fits.BinTableHDU.from_columns(cols)

	#If the file already exists, it appends HDUList file to it. If not, it saves a new file.
	if (existBool == True):
		#Loading FITS file
		fitsData = fits.open(outFile)

		#Appending HDUTable object to FITS data
		fitsData.append(hdu)

		#Saving FITS file
		fitsData.writeto(outFile, overwrite=True)

		#Closing FITS file
		fitsData.close()
	else:
		#Saving FITS file
		hdu.writeto(outFile)
	"""
