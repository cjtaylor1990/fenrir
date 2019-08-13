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

def rEvent(spin):
	return 1+np.sqrt(1-spin**2)

def findHeightIndex(spin,height,hMax=50,hNumCases=100):
	hMin = 1.1*rEvent(spin)
	j = (np.log10(height) - np.log10(hMin))/((np.log10(hMax)-np.log10(hMin))/(hNumCases-1))
	return int(j)+1

#Reading input parameters from command line: spin, corona_height, output_file, number_of_layers, file1, file2, ...
a = float(sys.argv[1]) #Spin of black hole (dimensionless)
hCorona = float(sys.argv[2]) #Height of corona along polar axis (r_g)
numThickness = int(sys.argv[3]) #Number for thicknesses used
filePath = str(sys.argv[4]) #File path to use for input and output files
inputFilePrefix = str(sys.argv[5]) #Prefix of input .npy files
outFilePrefix = str(sys.argv[6]) #Output FITS file
heightIndex = int(sys.argv[7])
#hMax = float(sys.argv[7])
print(sys.argv)

#Auto-generating list of input files based on prefix and number of thicknesses
inputFileList = ["{}{}{}.npy".format(filePath,inputFilePrefix,i) for i in range(numThickness)]

#Generating outfile name based on height value

#possibleHeights = {3.0:1, 5.0:2, 7.0:3, 10.0:4} #For test only. Once I have full scale, I need an analytic function
#heightIndex = possibleHeights[hCorona]
#heightIndex = findHeightIndex(a,hCorona)
outFile = filePath + outFilePrefix + str(heightIndex) + ".fits"

#Number of radial bins (relevant for FITS formating)
nRadBins = 150

#Checking to see if file already exists
existBool = os.path.isfile(outFile)

#If the file doesn't exist
if not existBool:
	for i in range(len(inputFileList)):
		if i != 0:
			fitsData = fits.open(outFile)
		currentFile = inputFileList[i]
		emData = np.load(currentFile)
		radius = emData[0][0] #Extracting radii values
		flux = emData[0][1] #Extracting flux v. radii
		delta = emData[0][2] #Extracting mean(delta) v. radii
		deltaInc = np.zeros(nRadBins)
		radiusColumn = fits.Column(name = 'r', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = [radius])
		fluxColumn = fits.Column(name = 'h{}'.format(heightIndex), format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = [flux])
		deltaColumn = fits.Column(name = 'del{}'.format(heightIndex), format = '{}E'.format(nRadBins), unit = 'deg', array = [delta])
		deltaIncColumn = fits.Column(name = 'del_inc{}'.format(heightIndex), format = '{}E'.format(nRadBins), unit = 'deg', array = [deltaInc])
		cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn,deltaIncColumn])
		hdu = fits.BinTableHDU.from_columns(cols)

		if i != 0:
			fitsData.append(hdu)
			fitsData.writeto(outFile,overwrite=True)
		else:
			hdu.writeto(outFile)


		del hdu

else:
	fitsData = fits.open(outFile)
	for i in range(len(inputFileList)):
		currentFile = inputFileList[i]
		emData = np.load(currentFile)
		radius = emData[0][0] #Extracting radii values
		flux = emData[0][1] #Extracting flux v. radii
		delta = emData[0][2] #Extracting mean(delta) v. radii
		deltaInc = np.zeros(nRadBins)

		currentHDU = fitsData[i+1].data
		currentRadius = [currentHDU[j][0] for j in range(len(currentHDU))]
		currentFlux = [currentHDU[j][1] for j in range(len(currentHDU))]
		currentDelta = [currentHDU[j][2] for j in range(len(currentHDU))]
		currentDeltaInc = [currentHDU[j][3] for j in range(len(currentHDU))]

		currentRadius.append(np.array(radius,dtype='float32'))
		currentFlux.append(np.array(flux,dtype='float32'))
		currentDelta.append(np.array(delta,dtype='float32'))
		currentDeltaInc.append(np.array(deltaInc,dtype='float32'))

		radiusColumn = fits.Column(name = 'r', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = currentRadius)
		fluxColumn = fits.Column(name = 'h1', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = currentFlux)
		deltaColumn = fits.Column(name = 'del1', format = '{}E'.format(nRadBins), unit = 'deg', array = currentDelta)
		deltaIncColumn = fits.Column(name = 'del_inc1', format = '{}E'.format(nRadBins), unit = 'deg', array = currentDeltaInc)
		cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn,deltaIncColumn])
		newHDU = fits.BinTableHDU.from_columns(cols)
		fitsData[i+1] = newHDU

	fitsData.writeto(outFile,overwrite=True)

"""
	fitsData = fits.open(outFile)

	for file in inputFileList[1:]:
		currentFile = inputFileList[0]
		emData = np.load(currentFile)
		radius = emData[0][0] #Extracting radii values
		flux = emData[0][1] #Extracting flux v. radii
		delta = emData[0][2] #Extracting mean(delta) v. radii
		deltaInc = np.zeros(nRadBins)
		radiusColumn = fits.Column(name = 'r', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = [radius])
		fluxColumn = fits.Column(name = 'h1', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = [flux])
		deltaColumn = fits.Column(name = 'del1', format = '{}E'.format(nRadBins), unit = 'deg', array = [deltaColumn])
		deltaIncColumn = fits.Column(name = 'del1', format = '{}E'.format(nRadBins), unit = 'deg', array = [deltaIncColumn])
		cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn,deltaIncColumn])
		fitsData.append(fits.BinTableHDU.from_columns(cols))

	fitsData.writeto(outFile,overwrite=True)
"""
"""
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

#Loading in data from .npy input file
data = np.load(inputFile) #Loading in file
emissivityData = data[0] #Extracting emissivity data
radius = emissivityData[0] #Extracting radii values
flux = emissivityData[1] #Extracting flux v. radii
delta = emissivityData[2] #Extracting mean(delta) v. radii

#If the file doesn't exist
if not existBool:
	radiusColumn = fits.Column(name = 'r', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = radius)
	fluxColumn = fits.Column(name = 'h1', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = [flux])
	deltaColumn = fits.Column(name = 'del1', format = '{}E'.format(nRadBins), unit = 'deg', array = [flux])
	cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn])
	hdu = fits.BinTableHDU.from_columns(cols)
	hdu.writeto(outFile)
else:
	fitsData = fits.open(outFile)
	if thicknessIndex+1 not in range(len(fitsData)):
		radiusColumn = fits.Column(name = 'r', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = radius)
		fluxColumn = fits.Column(name = 'h1', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = [flux])
		deltaColumn = fits.Column(name = 'del1', format = '{}E'.format(nRadBins), unit = 'deg', array = [flux])
		cols = fits.ColDefs([radiusColumn,fluxColumn,deltaColumn])
		hdu = fits.BinTableHDU.from_columns(cols)
		fitsData.append(hdu)
		fitsData.writeto(outFile,overwrite=True)

	else:
		currentHDU = fitsData[thicknessIndex+1].data
		radiusArray = [currentHDU[j][0] for j in range(len(currentHDU))]#.append(np.array(a,dtype='float32'))
		fluxArray = [currentHDU[j][1] for j in range(len(currentHDU))]#.append(np.array(b,dtype='float32'))
		deltaArray = [currentHDU[j][2] for j in range(len(currentHDU))]#.append(np.array(c,dtype='float32'))
		radiusArray.append(np.array(radius,dtype='float32'))
		fluxArray.append(np.array(flux,dtype='float32'))
		deltaArray.append(np.array(delta,dtype='float32'))
		radiusColumn = fits.Column(name = 'r', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = radiusArray)
		fluxColumn = fits.Column(name = 'h1', format = '{}E'.format(nRadBins), unit = 'GM/c^2', array = fluxArray)
		deltaColumn = fits.Column(name = 'del1', format = '{}E'.format(nRadBins), unit = 'deg', array = deltaArray)
		cols = fits.ColDefs([col1,col2,col3])
		currentHDU = fits.BinTableHDU.from_columns(cols)
		fitsData[thicknessIndex+1] = currentHDU
		fitsData.writeto(outFile,overwrite=True)
"""
"""
#If the file doesn't exist
if not existBool:
	data = np.load(file) #Loading in file
	emissivityData = data[0] #Extracting emissivity data
	radius = emissivityData[0] #Extracting radii values
	flux = emissivityData[1] #Extracting flux v. radii
	delta = emissivityData[2] #Extracting mean(delta) v. radii
	col1 = fits.Column(name = 'a', format = '100E', unit = 'n/a', array = aArray)
	col2 = fits.Column(name = 'h1', format = '150E', unit = 'GM/c^2', array = fluxArray)
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
