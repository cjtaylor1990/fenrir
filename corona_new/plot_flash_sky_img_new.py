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

def grSigma(r,a,theta):
	return (r**2.) + ((a*np.cos(theta))**2.)

def grDelta(r,a):
	return (r**2.) - (2.*r) + (a**2.)

def aFunct(r,a,theta):
	aFunctOut = (((r*r)+(a*a))**2.)-(a*a)*grDelta(r,a)*(np.sin(theta)**2.)
	return aFunctOut
"""
def grArea(r,theta,dr,a,dtheta):
	rTerm = (grSigma(r,a,theta)/grDelta(r,a)) + (grSigma(r,a,theta)*((dtheta/dr)**2.))
	phiTerm = (r**2.) + (a**2.) + (2.*r*((a*np.sin(theta))**2.)/grSigma(r,a,theta))
	areaOut = (2.*np.pi)*np.sqrt(rTerm*phiTerm)*dr
	return areaOut
"""
def grArea(r,theta,dr,a,dtheta):
	rTerm = np.sqrt((grSigma(r,a,theta)/grDelta(r,a)) + (grSigma(r,a,theta)*((dtheta/dr)**2.)*(np.sin(theta)**2.)))
	phiTerm = np.sqrt((aFunct(r,a,theta)/grSigma(r,a,theta)))#*(np.sin(theta)**2.))
	areaOut = (2.*np.pi)*phiTerm*rTerm*dr
	return areaOut

inFile = './test.npy'#str(sys.argv[1])
outFile = './test_hist.npy'#str(sys.argv[2])
nBins = 1000#int(sys.argv[3]) #1000
specIndex = 2.#float(sys.argv[5]) #2
#Unpacking data (x, y, gRatio, time, radius, theta, phi)
data = np.load(inFile)
x = data[0]
y = data[1]
gRatio = data[2]
time = data[3]
radius = data[4]
theta = data[5]
phi = data[6]
scaleHeight = data[7]
projectedRadius = data[8]
gamma = data[9]
diskHitSwitch = data[10]
print np.min(x)/np.pi
print np.max(x)/np.pi
a = 0.9
rIn = rIsco(a)
rOut = 100.#29.253597#float(sys.argv[4]) #100
print rIsco(a)

#iRcut = np.where(np.logical_and(projectedRadius > rIn,projectedRadius < rOut, diskHitSwitch > 0.5))
iRcut = np.where(np.logical_and((np.logical_and(np.logical_and(projectedRadius > rIn,projectedRadius < rOut), diskHitSwitch > 0)),(gRatio < 100.)))

"""
#cs = pl.contour(xPlot ,yPlot , rPlot, 20, colors = 'k')
pl.scatter(y[iRcut]/np.pi, x[iRcut]/np.pi, c = (projectedRadius)[iRcut], cmap = pl.cm.coolwarm_r, lw=0, edgecolor = 'none')
#pl.clabel(cs, inline = 1, fontsize = 10)
pl.xlabel(r'$\beta$', fontsize = 20)
pl.ylabel(r'$\alpha$', fontsize = 20)
pl.xlim([0.,2.])
pl.ylim([1.,0.])
#pl.text(80., 0.5, r'E$_{\rm obs}$ / E$_{\rm em}$', rotation = 270, fontsize = 20)
#pl.axhline(0., color = '0.7', linestyle = '--', linewidth = 3.)
pl.colorbar()
pl.show()
"""

specEnergy = (gRatio[iRcut])#[weirdCut])#[np.isfinite(gRatio[iRcut])]
specRadius = (projectedRadius[iRcut])#[weirdCut]#[np.isfinite(gRatio[iRcut])]
specTheta = (theta[iRcut])#[weirdCut]
specPhi = (phi[iRcut])#[weirdCut]
specGamma = (gamma[iRcut])#[weirdCut]

#binLimArray = np.logspace(np.log10(rIn),np.log10(rOut),nBins,endpoint=False)
binLimArray = np.linspace(rIn,rOut,nBins,endpoint=False)
specBin = np.zeros(nBins-1)
drArray = np.zeros(nBins-1)
dThetaArray = np.zeros(nBins-1)
numArray = np.zeros(nBins-1)
fluxArray = np.zeros(nBins-1)

l = 1
#print specHist[1]
while (l < nBins):
	rMinBin = binLimArray[l-1]
	rMaxBin = binLimArray[l]
	specBin[l-1] = (rMinBin+rMaxBin)/2.
	drArray[l-1] = rMaxBin-rMinBin
	iInBin = np.where(np.logical_and(specRadius > rMinBin, specRadius < rMaxBin))
	radInBin = specRadius[iInBin]
	#drArray[l-1] = np.max(radInBin)-np.min(radInBin)
	thetaInBin = specTheta[iInBin]
	gammaInBin = specGamma[iInBin]
	energyInBin = specEnergy[iInBin]
	numArray[l-1] = len(radInBin)
	if (numArray[l-1] > 0):
		#drArray[l-1] = np.max(radInBin)-np.min(radInBin)
		dThetaArray[l-1] = np.max(thetaInBin)-np.min(thetaInBin)
		grAreaInBin = grArea(radInBin,thetaInBin,drArray[l-1],a,dThetaArray[l-1])
		fluxInBin = (1./(gammaInBin*grAreaInBin))*(energyInBin**(specIndex))
		fluxArray[l-1] = np.sum(fluxInBin)
	else:
		fluxInBin = 0.
		fluxArray[l-1] = 0.
	l+=1

pl.figure(figsize=(10.,10.))
pl.plot(specBin,fluxArray, color = 'k', marker = 'o')
pl.axvline(rIn, color = 'g')
pl.xlabel('r')
pl.ylabel('F')
pl.yscale('log')
pl.xscale('log')
pl.xlim([rIn - 1.,100.])
outArray = np.array([[specBin,fluxArray,numArray],binLimArray])
np.save(outFile, outArray)
pl.show()
