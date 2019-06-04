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

#Reading in parameters from command line
a = float(sys.argv[1]) #Spin of black hole (dimensionless)
outFile = str(sys.argv[2]) #Output FITS files
numLayers = int(sys.argv[3]) #Number of .npy input files (one for each image size)
delRinput = float(sys.argv[4]) #Width of sample annulus for i-th image size is delR = dRcorrArray[i]*delRinput (delR = delRinput for smallest image size)
diskFileArray = [] #Will store paths to .npy input files
imgSizeArray = [] #Will store the image sizes for each item in diskFileArray
i = 0
j = 5
print(sys.argv)

#Reading in name of input .npy disk image file and its image size. Should be read from smallest to largest.
#I could put in a sort so that this is more general, thus do not have to read it in in any particular order
while (i < numLayers):
	print(sys.argv[j])
	print(sys.argv[j+1])
	diskFileArray.append(str(sys.argv[j]))
	imgSizeArray.append(float(sys.argv[j+1]))
	i+=1
	j+=2
imgSizeArray = np.array(imgSizeArray)

#HDU structure
nEnergyBins = 20 #Number of g* bins per entry
nRadialBins = 150 #Number of radial bins per HDU

#Calculating inner radius (assumed to be rIsco)
rIn = rIsco(a)
#print('rIsco ' + str(rIn))

#Calculating outer radius from image size array
rOutArray = (imgSizeArray/2.)-5.

#Defining the outer-most radius of the disk as the maximum of rOutArray
rOut = np.max(rOutArray)

#Correcting constants that modify dR for each image size
dRcorrArray = imgSizeArray/imgSizeArray[0]

#Correcting constants for the effective area per photon for each image size
corrArray = (imgSizeArray/np.max(imgSizeArray))**2.

#Creating array of sample radii (binned linearly by the square root)
#radialSampleArray = (np.linspace(np.sqrt(rIn),np.sqrt(rOut),nRadialBins,endpoint=True))**2.
radialSampleArray = np.logspace(np.log10(rIn),np.log10(rOut),nRadialBins,endpoint=True)

#Calculating the minimal distance between the different radial sample values. Use that for deltaR (delRinput)
#fineSampleArray = (np.linspace(np.sqrt(rIn),np.sqrt(rOut),2000,endpoint=True))**2.
delRarray = radialSampleArray[1:] - radialSampleArray[:-1]
minDelR = np.min(delRarray)
if (delRinput > minDelR):
	delRinput = minDelR
del delRarray
del minDelR

#Creating array of limits to radial sample "stripes"
radialBinMinArray = radialSampleArray
#radialBinMaxArray = radialSampleArray+delR
#print radialBinMinArray[0]


#Creating energy bins (units of g^star)
gStarLimArray = np.linspace(0.,1,nEnergyBins+1, endpoint=True) #Edges of g* bins
gStarMidArray = (gStarLimArray[1:]+gStarLimArray[:-1])/2. #Mid points of g* bins
dGstar = gStarLimArray[1:]-gStarLimArray[:-1] #delta-g* for each bin

#Unpacking data from disk transform data
diskData = np.load(diskFileArray[0])
x = diskData[0] #Horizontal position on the disk image (units of Rg)
y = diskData[1] #Vertical position on the disk image (Units of Rg)
gRatio = diskData[2] #Energy of the photon normalized to the rest energy (g = E/Erest)
time = diskData[3] #BL time of arrival (negative value since time is reversed, units of Rg/c)
radius = diskData[4] #BL spherical radius (units of Rg)
theta = diskData[5] #BL theta vertical angle (units of radians)
phi = diskData[6] #BL phi azimuthal angle (units of radians)
scaleHeight = diskData[7] #Scale height of disk at final photon position (units of Rg)
projectedRadius = diskData[8] #Cylindrical radius (units of Rg)

#Finding dA and dR correction factors for first image
correction = corrArray[0]
dRcorrection = dRcorrArray[0]

#Calculating delR based on current dRcorrection
delR = delRinput*dRcorrection

#Finding the outer radii for each annulus for given image
radialBinMaxArray = radialSampleArray+delR

#Correcting phi values to be from 0 to 2pi.
i = 0
while (i < len(phi)):
	val = phi[i]
	if (val < 0.):
		valFix = (2.*np.pi) - (np.abs(phi[i])%(2.*np.pi))
		phi[i] = valFix
	else:
		phi[i] = val%(2.*np.pi)
	i+=1

#Creating index array for disk data within range between rIn and rOut
iRcut = np.where(np.logical_and(projectedRadius >= rIn,projectedRadius < rOut+delR))

#Creating arrays with radius cut from iRcut
specX = x[iRcut]
specY = y[iRcut]
specRadius = projectedRadius[iRcut]
specEnergy = gRatio[iRcut]
specPhi = phi[iRcut]
specHeight = scaleHeight[iRcut]

#Creating data storage array
rArray = np.zeros(nRadialBins)
gMinArray = np.zeros(nRadialBins)
gMaxArray = np.zeros(nRadialBins)
tranArray1 = np.zeros((nRadialBins,nEnergyBins))
tranArray2 = np.zeros((nRadialBins,nEnergyBins))

i=0
fileNumber = 0
print(diskFileArray)
print(rOutArray)
while (i < nRadialBins):
	#Finding radial limits and mid point of annulus, along with radial width
	rMinBin = radialBinMinArray[i]
	rMaxBin = radialBinMaxArray[i]
	#rMidBin = radialSampleArray[i]
	#print(i)
	print ('i: ' + str(i))
	if (rMinBin > rOutArray[fileNumber]) and (fileNumber+1 < len(diskFileArray)):
		#Incrementing file number
		fileNumber += 1
		print("Switching files...")

		#Unpacking data from disk transform data
		print(fileNumber)
		diskData = np.load(diskFileArray[fileNumber])
		x = diskData[0]
		y = diskData[1]
		gRatio = diskData[2]
		time = diskData[3]
		radius = diskData[4]
		theta = diskData[5]
		phi = diskData[6]
		scaleHeight = diskData[7]
		projectedRadius = diskData[8]

		correction = corrArray[fileNumber]

		#Finding dA and dR correction factors for first image
		correction = corrArray[fileNumber]
		dRcorrection = dRcorrArray[fileNumber]

		#Calculating delR based on current dRcorrection
		delR = delRinput*dRcorrection

		#Finding the outer radii for each annulus for given image
		radialBinMaxArray = radialSampleArray+delR

		#Finding new rMax for current bin based on new radialBinMaxArray
		rMaxBin = radialBinMaxArray[i]

		j = 0
		while (j < len(phi)):
			val = phi[j]
			if (val < 0.):
				valFix = (2.*np.pi) - (np.abs(phi[j])%(2.*np.pi))
				phi[j] = valFix
			else:
				phi[j] = val%(2.*np.pi)
			j+=1

		#Creating index array for disk data within range between rIn and rOut
		print(projectedRadius)
		print(np.min(projectedRadius))
		print(np.max(projectedRadius))
		iRcut = np.where(np.logical_and(projectedRadius >= rIn,projectedRadius < rOut+delR))

		#Creating arrays with radius cut from iRcut
		specX = x[iRcut]
		specY = y[iRcut]
		specRadius = projectedRadius[iRcut]
		specEnergy = gRatio[iRcut]
		specPhi = phi[iRcut]
		specHeight = scaleHeight[iRcut]

	#print(specRadius)
	print ('rMin: ' + str(rMinBin))
	print ('rMax: ' + str(rMaxBin))
	print ('rOutFile: ' + str(rOutArray[fileNumber]))

	#Finding indices for photons that fall in annulus
	iInBin = np.where(np.logical_and(specRadius >= rMinBin, specRadius < rMaxBin))

	#Finding energies of photons that fall in annulus
	gBin = specEnergy[iInBin]
	phiBin = specPhi[iInBin]
	rBin = specRadius[iInBin]
	xBin = specX[iInBin]
	yBin = specY[iInBin]
	heightBin = specHeight[iInBin]

	#print(gBin)

	if (len(gBin) > 1):
		#Calculating g_min, g_max
		#print(len(gBin))
		#print(rMinBin)
		gMin = np.min(gBin)
		gMax = np.max(gBin)
		gStar = (gBin - gMin)/(gMax - gMin)
		#print(len(gBin))

		#print(np.where(np.isnan(gBin) == True))
		#print(rBin[np.where(np.isnan(gBin) == True)])
		#print(heightBin[np.where(np.isnan(gBin) == True)])

		#print(phiBin[np.where(gBin == gMin)])
		#Finding the phi value corresponding to g_min and g_max
		phiGmin = np.min(phiBin[np.where(gBin == gMin)])
		phiGmax = np.max(phiBin[np.where(gBin == gMax)])


		#Finding two "branches" of the transfer function (front and back sides of disk)
		iBack = np.where(np.logical_and(phiBin >= phiGmin, phiBin < phiGmax))
		iFront = np.where(np.logical_not(np.logical_and(phiBin >= phiGmin, phiBin < phiGmax)))

		#Finding normal g's for back and front of the disk
		gBack = gBin[iBack]
		gFront = gBin[iFront]

		#Finding g^star for back and front of disk
		gStarBack = gStar[iBack]
		gStarFront = gStar[iFront]

		rBack = rBin[iBack]
		rFront = rBin[iFront]

		#Calculating flux of photons in annulus as function of energy for both branches
		fluxBack = (gBack**3.)*correction
		fluxFront = (gFront**3.)*correction

		#Calculating the g array to calculate the energy-dependent flux
		gLims = (gMax-gMin)*gStarLimArray + gMin
		gMids = (gMax - gMin)*gStarMidArray + gMin
		dG = gLims[1] - gLims[0]

		transferIndivBack = (fluxBack/(rBack*delR))*(np.sqrt(gStarBack*(1-gStarBack))/(gBack**2.))
		transferIndivFront = (fluxFront/(rFront*delR))*(np.sqrt(gStarFront*(1-gStarFront))/(gFront**2.))

		transferFront = (np.histogram(gStarFront, gStarLimArray, weights = transferIndivFront)[0])/dGstar
		transferBack = (np.histogram(gStarBack, gStarLimArray, weights = transferIndivBack)[0])/dGstar

		#Normalizing the transfer functions
		norm = 1.#np.sum(transferBack)+np.sum(transferFront)
		transferBack = transferBack/norm
		transferFront = transferFront/norm

		#Calculating the energy-dependent flux for both branches
		#totFluxBack = np.histogram(gBack, gLims, weights = fluxBack*correction)
		#totFluxFront = np.histogram(gFront, gLims, weights = fluxFront*correction)

		#Calculating transfer function for annulus for both branches
		#transferBack = (totFluxBack[0]/(rBin*delR))*((gMax-gMin)/(gMids**3.))*np.sqrt(gStarMidArray*(1.-gStarMidArray))
		#transferFront = (totFluxFront[0]/(rBin*delR))*((gMax-gMin)/(gMids**3.))*np.sqrt(gStarMidArray*(1.-gStarMidArray))
	else:
		transferBack = np.zeros(nEnergyBins)
		transferFront = np.zeros(nEnergyBins)
		gMin = 0.
		gMax = 0.
		#print('Empty Bin')
		#print(rMinBin)
		#print(rMaxBin)

		#Normalizing the transfer functions
		norm = 1.#np.sum(transferBack)+np.sum(transferFront)
		transferBack = transferBack/norm
		transferFront = transferFront/norm

	#Creating data array for annulus
	rArray[i] = np.float32(rMinBin)
	gMinArray[i] = np.float32(gMin)
	gMaxArray[i] = np.float32(gMax)
	tranArray1[i] = np.array(transferFront)
	tranArray2[i] = np.array(transferBack)

	i+=1

#Creating column objects
col1 = fits.Column(name = 'r', format = '1E', unit = 'GM/c^2', array = rArray[::-1])
col2 = fits.Column(name = 'gmin', format = '1E', array = gMinArray[::-1])
col3 = fits.Column(name = 'gmax', format = '1E', array = gMaxArray[::-1])
col4 = fits.Column(name = 'trff1', format = '20E', unit = 'k=1', array = tranArray1[::-1])
col5 = fits.Column(name = 'trff2', format = '20E', unit = 'k=2', array = tranArray2[::-1])

#Combining column objects
cols = fits.ColDefs([col1, col2, col3, col4, col5])

#Creating HDUTable
hdu = fits.BinTableHDU.from_columns(cols)

#Checking to see if file already exists
existBool = os.path.isfile(outFile)

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
