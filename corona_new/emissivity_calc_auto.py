import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
import csv
import sys

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

#Input argv: inFile, outFile, nBins, spin(a), corona_height(hCorona)
print(sys.argv[1:])
filePath = sys.argv[1]
inFilePrefix = sys.argv[2]
outFilePrefix = sys.argv[3]
thicknessIndex = int(sys.argv[4])

a = float(sys.argv[5])#0.998
hCorona = float(sys.argv[6])
nBins = int(sys.argv[7])#150

inFile = filePath + inFilePrefix + str(thicknessIndex) + '.npy'#'./test_even.npy'
outFile = filePath + outFilePrefix + str(thicknessIndex) + '.npy'#'./test_hist.npy'
#nBins = int(sys.argv[3])#150
specIndex = 2.
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
#print np.min(x)/np.pi
#print np.max(x)/np.pi

rIn = rIsco(a)
rOut = 500.
#print rIsco(a)

#iRcut = np.where(np.logical_and(projectedRadius > rIn,projectedRadius < rOut, diskHitSwitch > 0.5))
iRcut = np.where(np.logical_and((np.logical_and(np.logical_and(projectedRadius[:-1] > rIn,projectedRadius[:-1] < rOut), diskHitSwitch[:-1] > 0)),(gRatio[:-1] < 100.)))

specEnergy = (gRatio)[iRcut]#[weirdCut])#[np.isfinite(gRatio[iRcut])]
specRadius = (projectedRadius)[iRcut]#[weirdCut]#[np.isfinite(gRatio[iRcut])]
specTheta = (theta)[iRcut]#[weirdCut]
specPhi = (phi)[iRcut]#[weirdCut]
specGamma = (gamma)[iRcut]#[weirdCut]
specDelta = np.pi - x[iRcut]

#binLimArray = np.logspace(np.log10(rIn),np.log10(rOut),nBins,endpoint=False)
logBinWidth = (np.log10(rOut) - np.log10(rIn))/(nBins-1)
binLimArray = [np.log10(rIn) + logBinWidth*i for i in range(nBins)]
binLimArray.append(np.log10(rOut)+logBinWidth)
binLimArray = 10.**(np.array(binLimArray))
#binLimArray = np.linspace(rIn,rOut,nBins,endpoint=False)
specBin = np.zeros(nBins)
drArray = np.zeros(nBins)
dThetaArray = np.zeros(nBins)
numArray = np.zeros(nBins)
fluxArray = np.zeros(nBins)
dDelta = np.zeros(nBins)
binDelta = np.zeros(nBins)

l = 0
#print specHist[1]
while (l < nBins):
	rMinBin = binLimArray[l]
	rMaxBin = binLimArray[l+1]
	specBin[l] = (rMinBin+rMaxBin)/2.
	drArray[l] = rMaxBin-rMinBin
	iInBin = np.where(np.logical_and(specRadius > rMinBin, specRadius < rMaxBin))
	radInBin = specRadius[iInBin]
	#drArray[l-1] = np.max(radInBin)-np.min(radInBin)
	thetaInBin = specTheta[iInBin]
	gammaInBin = specGamma[iInBin]
	energyInBin = specEnergy[iInBin]
	deltaInBin = specDelta[iInBin]
	numArray[l] = len(radInBin)
	if (numArray[l] > 0):
		#drArray[l-1] = np.max(radInBin)-np.min(radInBin)
		dThetaArray[l] = np.max(thetaInBin)-np.min(thetaInBin)
		dDelta[l] = np.max(deltaInBin) - np.min(deltaInBin)
		binDelta[l] = np.mean(deltaInBin)
		grAreaInBin = grArea(radInBin,thetaInBin,drArray[l],a,dThetaArray[l])
		fluxInBin = (1./(gammaInBin*grAreaInBin))*np.sin(deltaInBin)#*(energyInBin**(specIndex))#*np.sin(deltaInBin)
		fluxArray[l] = np.sum(fluxInBin)*drArray[l]
	else:
		fluxInBin = 0.
		fluxArray[l] = 0.
		binDelta[l] = 0.
	l+=1

outArray = np.array([[binLimArray[:-1],fluxArray,binDelta],a,hCorona])
np.save(outFile, outArray)
