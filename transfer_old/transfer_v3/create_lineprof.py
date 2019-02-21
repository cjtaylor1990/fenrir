import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
import csv
import sys

def rIsco(a):
	z1 = 1 + (((1-(a*a))**(1./3.))*(((1+a)**(1./3.))+((1-a)**(1./3.))))
	z2 = ((3*(a*a))+(z1*z1))**0.5
	rOut = 3+z2-(((3-z1)*(3+z1+(2.*z2)))**0.5)
	return rOut

a = 0.998
emPower = -3.
nEnergyBins = 200#1000
#rIn = rIsco(a)

filePath = './'
fileNames = [filePath + 'test1.npy',filePath+'test2.npy',filePath+'test3.npy']
outFile = filePath + 'transfer_test_directflux.npy'
rInVals = [rIsco(a),10.,30.]
rOutVals = [10.,30.,100.]
solidAngCorrect = [(1./49.),(1./9.),1.]

energyBins = np.linspace(0.002, 2.002, nEnergyBins+1)
midEnergy = (energyBins[1:] + energyBins[:-1])/2.

lineProfile = np.zeros(nEnergyBins)

i = 0
while (i < len(fileNames)):
	diskFile = fileNames[i]
	rIn = rInVals[i]
	rOut = rOutVals[i]
	correction = solidAngCorrect[i]

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

	#Creating index array for disk data within range between rIn and rOut
	iRcut = np.where(np.logical_and(projectedRadius >= rIn,projectedRadius < rOut))

	#Creating arrays with radius cut from iRcut
	specRadius = projectedRadius[iRcut]
	specEnergy = gRatio[iRcut]
	xRcut = x[iRcut]
	yRcut = y[iRcut]
	
	#Creating line profile via histogram
	indivFlux = (specEnergy**3.)*(specRadius**emPower)*correction
	lineProfile = lineProfile + (np.histogram(specEnergy, bins = energyBins, weights = indivFlux))[0]
	
	i += 1

pl.figure(figsize=(10.,10.))
pl.plot(midEnergy, lineProfile/np.sum(lineProfile), color = 'k', linestyle = '-')
pl.xlabel(r'$g \equiv E_{\rm obs} / E_{\rm em}$')
pl.ylabel(r'$\Phi$ [Arbitrary Units]')
pl.xlim([0.1,2.])
pl.show()

np.save(outFile, np.array([midEnergy,lineProfile]))

