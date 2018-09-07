import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
import csv
import sys

#Function that calculates Risco(a)
def rIsco(a):
	z1 = 1 + (((1-(a*a))**(1./3.))*(((1+a)**(1./3.))+((1-a)**(1./3.))))
	z2 = ((3*(a*a))+(z1*z1))**0.5
	rOut = 3+z2-(((3-z1)*(3+z1+(2.*z2)))**0.5)
	return rOut

a = 0.9 #Blackhole spin parameter
diskFile = './test_all.npy' #Disk image data file

n = 1000.
#imgSize = 70.
#dx = imgSize/n
#dy = dx
nEnergyBins = 1000 #Number of spectral energy bins
nRadBins = 1000 #Number of radial bins for emissivity power-law
rIn = rIsco(a) #Calculating Risco
rOut = 30.#120.#29.253597 #Outer radius of the accretion disk

print rIn

specEnergyLimsLow = 0.002
specEnergyLimsHigh = 2.002

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

print np.min(radius)
"""
xPlot = np.zeros([n,n])
yPlot = np.zeros([n,n])
rPlot = np.zeros([n,n])
i = 0
j = 0
while (j<n):
	k = 0
	while (k<n):
		xPlot[j,k] = x[i]
		yPlot[j,k] = y[i]
		rPlot[j,k] = projectedRadius[i]
		i+=1
		k+=1
	j+=1
"""
print x
print np.min(x)
print np.max(x)
#Creating index array for disk data within range between rIn and rOut
iRcut = np.where(np.logical_and(projectedRadius > rIn,projectedRadius < rOut))

#Creating arrays with radius cut from iRcut
specRadius = projectedRadius[iRcut]
specEnergy = gRatio[iRcut]
specX = x[iRcut]
specY = y[iRcut]
print np.min(specRadius)

#Creating energy grid limts (units of g)
specEnergyLims = np.linspace(specEnergyLimsLow, specEnergyLimsHigh, nEnergyBins+1)
specEnergyGrid = np.zeros(nEnergyBins)

indivFlux = []
indivEnergy = []
xDisk = []
yDisk = []

i = 0
while (i < nEnergyBins):
	eMinBin = specEnergyLims[i]
	eMaxBin = specEnergyLims[i+1]
	specEnergyGrid[i] = (eMinBin+eMaxBin)/2.
	iInBin = np.where(np.logical_and(specEnergy > eMinBin, specEnergy < eMaxBin))
	radInBin = specRadius[iInBin]
	xInBin = specX[iInBin]
	yInBin = specY[iInBin]
	eInBin = specEnergy[iInBin]
	indivFlux.extend((radInBin**-3.)*(eInBin**3.))
	indivEnergy.extend(eInBin)
	xDisk.extend(xInBin)
	yDisk.extend(yInBin)
	i+=1

specHist = np.histogram(indivEnergy, specEnergyLims, weights = indivFlux)
"""
#pl.figure(figsize=(10.,10.))
pl.plot(specEnergyGrid, (specHist[0])/np.sum(specHist[0]), color = 'k', linestyle = '-')
pl.xlabel(r'$g \equiv E_{\rm obs} / E_{\rm em}$')
pl.ylabel(r'$\Phi$ [Arbitrary Units]')
#pl.xlim([0.1,1.2])

pl.show()
"""
pl.figure(figsize=(8.,8.))
#cs = pl.contour(xPlot ,yPlot , rPlot, 20, colors = 'k')
#pl.scatter(xDisk, yDisk, c = np.log10(np.array(indivFlux)+1.), cmap = pl.get_cmap('gnuplot2'), lw=0, edgecolor = 'none')
pl.scatter(xDisk, yDisk, c = np.array(indivEnergy), cmap = pl.get_cmap('gist_rainbow'), lw=0, edgecolor = 'none')
#pl.clabel(cs, inline = 1, fontsize = 15)
pl.xlabel(r'x ($r_{\rm g}$)', fontsize = 20)
pl.ylabel(r'y ($r_{\rm g}$)', fontsize = 20)
pl.xlim([-35.,35.])
pl.ylim([-35.,35.])
#pl.text(52., 8, r"log$_{10}$(g)", rotation = 270, fontsize = 20)
#pl.text(52., 8, r"log$_{10}$[$I(\rm{x,y})$ + 1]", rotation = 270, fontsize = 20)
pl.colorbar()
pl.clim(0.,1.2)
pl.show()

pl.plot(specEnergyGrid,specHist[0])
pl.show()
