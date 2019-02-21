import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

def rIsco(a):
	z1 = 1 + (((1-(a*a))**(1./3.))*(((1+a)**(1./3.))+((1-a)**(1./3.))))
	z2 = ((3*(a*a))+(z1*z1))**0.5
	rOut = 3+z2-(((3-z1)*(3+z1+(2.*z2)))**0.5)
	return rOut
a = float(sys.argv[1])#0.9981

diskFile = str(sys.argv[2])#'../../../transfer_tests/transfer.npy'
outFile = str(sys.argv[3])#'./transfer_grid.fits'

nEnergyBins = 50
nRadialBins = 50
rIn = rIsco(a)
rOut =30.

#Unpacking data from disk transform data
diskData = np.load(diskFile)
x = diskData[0]
y = diskData[1]
gRatio = diskData[2]
time = diskData[3]
radius = diskData[4]
theta = diskData[5]
phi = diskData[6]
scaleHeight = diskData[7]
projectedRadius = diskData[8]

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
iRcut = np.where(np.logical_and(projectedRadius > rIn,projectedRadius < rOut))

#Creating arrays with radius cut from iRcut
specX = x[iRcut]
specY = y[iRcut]
specRadius = projectedRadius[iRcut]
specEnergy = gRatio[iRcut]
specPhi = phi[iRcut]

#Creating radial bins
radialBinLimArray = (np.linspace(np.sqrt(rIn),np.sqrt(rOut),nRadialBins+1,endpoint=True))**2.
radialBinMidArray = (radialBinLimArray[1:] + radialBinLimArray[:-1])/2.
delRarray = radialBinLimArray[1:] - radialBinLimArray[:-1]

#Creating energy bins (units of g^star)
gStarLimArray = np.linspace(0.,1.,nEnergyBins+1, endpoint=True)
gStarMidArray = (gStarLimArray[1:]+gStarLimArray[:-1])/2.

#Creating data storage array
rArray = np.zeros(nRadialBins)
gMinArray = np.zeros(nRadialBins)
gMaxArray = np.zeros(nRadialBins)
tranArray1 = np.zeros((nRadialBins,nEnergyBins))
tranArray2 = np.zeros((nRadialBins,nEnergyBins))

i = 0
while (i < nRadialBins):
	#Finding radial limits and mid point of annulus, along with radial width
	rMinBin = radialBinLimArray[i]
	rMaxBin = radialBinLimArray[i+1]
	rBin = radialBinMidArray[i]
	delR = delRarray[i]
	
	#Finding indices for photons that fall in annulus
	iInBin = np.where(np.logical_and(specRadius > rMinBin, specRadius < rMaxBin))
	
	#Finding energies of photons that fall in annulus
	gBin = specEnergy[iInBin]
	phiBin = specPhi[iInBin]
	xBin = specX[iInBin]
	yBin = specY[iInBin]
	
	#Calculating g_min, g_max
	gMin = np.min(gBin)
	gMax = np.max(gBin)
	gStar = (gBin - gMin)/(gMax - gMin)
	
	#Finding the phi value corresponding to g_min and g_max
	phiGmin = (phiBin[np.where(gBin == gMin)])[0]
	phiGmax = (phiBin[np.where(gBin == gMax)])[0]
	
	#Finding two "branches" of the transfer function (front and back sides of disk)
	iBack = np.where(np.logical_and(phiBin > phiGmin, phiBin < phiGmax))
	iFront = np.where(np.logical_not(np.logical_and(phiBin > phiGmin, phiBin < phiGmax)))
	
	#Finding normal g's for back and front of the disk
	gBack = gBin[iBack]
	gFront = gBin[iFront]
	
	#Finding g^star for back and front of disk
	gStarBack = gStar[iBack]
	gStarFront = gStar[iFront]
	
	#Calculating flux of photons in annulus as function of energy for both branches
	fluxBack = gBack**3.
	fluxFront = gFront**3.
	
	#Calculating the g array to calculate the energy-dependent flux
	gLims = (gMax-gMin)*gStarLimArray + gMin
	gMids = (gMax - gMin)*gStarMidArray + gMin
	dG = gLims[1] - gLims[0]
	
	#Calculating the energy-dependent flux for both branches
	totFluxBack = np.histogram(gBack, gLims, weights = fluxBack)
	totFluxFront = np.histogram(gFront, gLims, weights = fluxFront)
	
	#Calculating transfer function for annulus for both branches
	transferBack = (totFluxBack[0]/(rBin*delR))*((gMax-gMin)/(gMids**2.))*np.sqrt(gStarMidArray*(1.-gStarMidArray))
	transferFront = (totFluxFront[0]/(rBin*delR))*((gMax-gMin)/(gMids**2.))*np.sqrt(gStarMidArray*(1.-gStarMidArray))
	
	#Normalizing the transfer functions
	norm = np.sum(transferBack)+np.sum(transferFront)
	transferBack = transferBack/norm
	transferFront = transferFront/norm
	
	#Creating data array for annulus
	rArray[i] = np.float32(rBin)
	gMinArray[i] = np.float32(gMin)
	gMaxArray[i] = np.float32(gMax)
	tranArray1[i] = np.array(transferFront)
	tranArray2[i] = np.array(transferBack)
	
	i+=1

#Creating column objects
col1 = fits.Column(name = 'r', format = '1E', unit = 'GM/c^2', array = rArray)
col2 = fits.Column(name = 'gmin', format = '1E', array = gMinArray)
col3 = fits.Column(name = 'gmax', format = '1E', array = gMaxArray)
col4 = fits.Column(name = 'trff1', format = '50E', unit = 'k=1', array = tranArray1)
col5 = fits.Column(name = 'trff2', format = '50E', unit = 'k=2', array = tranArray2)

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