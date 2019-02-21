import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from astropy.io import fits
import csv
import sys
import os

inFile = str(sys.argv[1])
tableNum = int(sys.argv[2])
rIn = float(sys.argv[3])
rOut = float(sys.argv[4])
emmPower = float(sys.argv[5])

#Loading in FITS file
hdul = fits.open(inFile)

#Getting specific table data from FITS data
hdr = hdul[tableNum]
allRadius = hdr.data.field('r')
iRadCut = np.where(np.logical_and(allRadius >= rIn, allRadius < rOut))
radius = allRadius[iRadCut]
gMin = (hdr.data.field('gmin'))[iRadCut]
gMax = (hdr.data.field('gmax'))[iRadCut]
transfer1 = (hdr.data.field('trff1'))[iRadCut]
transfer2 = (hdr.data.field('trff2'))[iRadCut]

#Constructing gStar array
nEnergyBins = 50
gStarLimArray = np.linspace(0.,1.,nEnergyBins+1, endpoint=True)
gStarMidArray = (gStarLimArray[1:]+gStarLimArray[:-1])/2.

#Calculating dr array
nRadialBins = 50
radialBinLimArray = (np.linspace(np.sqrt(rIn),np.sqrt(rOut),nRadialBins+1,endpoint=True))**2.
dr = radialBinLimArray[1:] - radialBinLimArray[:-1]

#Constructing g array
gLimArray = np.linspace(0.002,2.002,nEnergyBins+1,endpoint=True)
gMidArray = (gLimArray[1:]+gLimArray[:-1])/2.

#Constructing line profile array
lineprof = np.zeros(nEnergyBins)

#Looping over radius
i = 0
while (i < len(radius)):
	gMinBin = gMin[i]
	gMaxBin = gMax[i]
	gArrayBin = ((gMaxBin-gMinBin)*gStarMidArray)+gMinBin
	transferBin = transfer1[i] + transfer2[i]
	#emmBin = radius**(-1.*emmPower)
	drBin = dr[i]
	rBin = radius[i]
	emmBin = rBin**(-1.**emmPower)
	fluxBin = ((gArrayBin**2.)/np.sqrt(gStarMidArray*(1.-gStarMidArray)))*(transferBin/(gMaxBin-gMinBin))*emmBin*rBin*drBin
	fluxHist = np.histogram(gArrayBin,gLimArray,weights=fluxBin)
	lineprof = lineprof+fluxHist[0]
	i+=1
pl.plot(gMidArray,lineprof)
pl.xscale('log')
pl.yscale('log')
pl.show()
np.save('construct_lineprof.npy',np.array([gMidArray,lineprof]))

