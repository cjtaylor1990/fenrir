import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib
from matplotlib.colors import LogNorm
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

def grArea(r,theta,dr,a,dtheta,dPhi):
	rTerm = np.sqrt((grSigma(r,a,theta)/grDelta(r,a)) + (grSigma(r,a,theta)*((dtheta/dr)**2.)*(np.sin(theta)**2.)))
	phiTerm = np.sqrt((aFunct(r,a,theta)/grSigma(r,a,theta)))#*(np.sin(theta)**2.))
	areaOut = phiTerm*rTerm*dr*dPhi
	return areaOut

def angmomF(r,a,theta,imgAlpha,imgBeta):
	top = np.sin(theta)*np.sin(imgAlpha)*np.sin(imgBeta)
	aF = (((r*r)+(a*a))**2.)-(grDelta(r,a)*a*a*(np.sin(theta)**2.))
	eps = (np.sqrt(grSigma(r,a,theta)*grDelta(r,a)) + ((2*a*r/np.sqrt(grSigma(r,a,theta)))*top))/np.sqrt(aF)
	return (np.sqrt(aF)*top)/(np.sqrt(grSigma(r,a,theta))*eps)
	
def carterF(r,a,theta,imgAlpha,imgBeta):
	angmomVal = angmomF(r,a,theta,imgAlpha,imgBeta)
	top = np.sin(theta)*np.sin(imgAlpha)*np.sin(imgBeta)
	aF = (((r*r)+(a*a))**2.)-(grDelta(r,a)*a*a*(np.sin(theta)**2.))
	epsVal = (np.sqrt(grSigma(r,a,theta)*grDelta(r,a)) + ((2*a*r/np.sqrt(grSigma(r,a,theta)))*top))/np.sqrt(aF)
	grSigmaVal = grSigma(r,a,theta)
	grDeltaVal = grDelta(r,a)
	firstTerm = (((r*r)+(a*a)-(a*angmomVal))**2.)/grDeltaVal
	secondTerm = (grSigmaVal*(np.cos(imgAlpha)**2.))/(epsVal**2.)
	thirdTerm = (angmomVal**2.) - (2.*a*angmomVal) + (a*a)
	return firstTerm - secondTerm - thirdTerm
	
#Input and output files
inFile = './all.npy' #input corona npy file
outTimeFile = './all_time.npy' #output file for time map
outFluxFile = './all_irr.npy' #output file for irradiation map
irrFigFile = './all_irr.png' #output png file of irradiation map

#Input parameters
a = 0.9 #black hole spin
specIndex = 2. #corona spectral index
rOut = 50. #outer radius of disk
rIn = rIsco(a) #inner radius of disk (currently isco)

#Binning information
rBins = 500 #number of radial bins
phiBins = 500 #number of phi bins

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

#Fixing phi information to be within range of 0-2pi
i = 0
while (i < len(phi)):
	val = phi[i]
	if (val < 0.):
		valFix = (2.*np.pi) - (np.abs(phi[i])%(2.*np.pi))
		phi[i] = valFix
	else:
		phi[i] = val%(2.*np.pi)
	i+=1

#Finding photons and that hit the disk and fall within the user-inputted limits of the disk radii
iRcut = np.where(np.logical_and(np.logical_and(projectedRadius > rIn,projectedRadius < rOut),diskHitSwitch > 0))

#Finding photon info that falls on disk
specEnergy = gRatio[iRcut]
specRadius = projectedRadius[iRcut]
specTheta = theta[iRcut]
specPhi = phi[iRcut]
specTime = time[iRcut]
specGamma = gamma[iRcut]
specY = y[iRcut]
specX = x[iRcut]
print np.min(specRadius)
print np.min(specTime)
print np.max(specTime)
print np.median(specTime)

#Defining arrays of bin limits
rLimArray = np.linspace(rIn,rOut,rBins+1,endpoint=True)
phiLimArray = np.linspace(0.,2.*np.pi,phiBins+1,endpoint=True)

#Defining arrays of 0s that will be used to store information
#Binning info arrays
midRbin = np.zeros(rBins)
midPhiBin = np.zeros([rBins,phiBins]) 
drArray = np.zeros(rBins)

#Flux info arrays
numArray = np.zeros([rBins,phiBins])
fluxArray = np.zeros([rBins,phiBins])
fluxRarray = np.zeros(rBins)
fluxIndiv = np.zeros(len(specRadius))

#Time info arrays
timeIndiv = np.zeros(len(specTime))
timeArray = np.zeros([rBins,phiBins])

#Defining dummy variables 
i = 0

#Starting sorting loops
while (i < rBins): #Radial loop
	#Calculating r bin information (annulus)
	rMin = rLimArray[i]
	rMax = rLimArray[i+1]
	midRbin[i] = (rMin+rMax)/2.
	drArray[i] = rMax-rMin
	
	#Creating temporary arrays to fill with flux, number, and time info
	fluxTempArray = np.zeros(phiBins)
	numTempArray = np.zeros(phiBins)
	midPhiTempArray = np.zeros(phiBins)
	timeTempArray = np.zeros(phiBins)
	
	#Creating index array for annulus with rMin < r < rMax
	iRcut = np.where(np.logical_and(specRadius > rMin, specRadius < rMax))
	
	#Finding info for photons that land in annulus
	radRbin = specRadius[iRcut]
	thetaRbin = specTheta[iRcut]
	phiRbin = specPhi[iRcut]
	timeRbin = specTime[iRcut]
	gammaRbin = specGamma[iRcut]
	energyRbin = specEnergy[iRcut]
	
	#Defining j dummy variable and setting it equal to 0
	j = 0
	
	while (j < phiBins): #Phi loop
		#Calculating phi bin infomation
		phiMin = phiLimArray[j]
		phiMax = phiLimArray[j+1]
		midPhiTempArray[j] = (phiMin+phiMax)/2.
		
		#Creating index array for phi bin with phiMin < phi < phiMax
		phiBinCut = np.where(np.logical_and(phiRbin > phiMin, phiRbin < phiMax))
		
		#Finding photon information for those that fall within phi bin
		radPhiBin = radRbin[phiBinCut]
		thetaPhiBin = thetaRbin[phiBinCut]
		timePhiBin = timeRbin[phiBinCut]
		gammaPhiBin = gammaRbin[phiBinCut]
		energyPhiBin = energyRbin[phiBinCut]
		
		#Calculating the number of photons in the phi bin
		numTempArray[j] = len(radPhiBin)
		
		#Checking the number of photons that fall in phi bin
		if (numTempArray[j] > 0): #If photon number not zero
			#Calculating dTheta and dPhi for the phi bin
			dTheta = np.max(thetaPhiBin) - np.min(thetaPhiBin)
			dPhi = phiMax - phiMin
			
			#Calculating the area of the bin
			grAreaPhiBin = grArea(radPhiBin,thetaPhiBin,drArray[i],a,dTheta,dPhi)
			
			#Calculating the photon flux in the phi bin
			fluxPhiBin = (1./(gammaPhiBin*grAreaPhiBin))*(energyPhiBin**(specIndex))
			
			#Recording total photon flux in the phi bin
			fluxTempArray[j] = np.sum(fluxPhiBin)
			
			#Recording median photon travel time
			timeTempArray[j] = np.median(timePhiBin)
			
			#Recording individual photon flux and travel time for each photons
			iBinCutFull = np.where(np.logical_and(np.logical_and(np.logical_and(specRadius > rMin, specRadius < rMax),specPhi > phiMin),specPhi < phiMax))
			fluxIndiv[iBinCutFull] = fluxPhiBin
			timeIndiv[iBinCutFull] = timePhiBin
			
			#Incrementing j dummy variable
			j+=1
		else: #If photon number is 0
			#incrementing j dummy variable
			j+=1
	
	#Recording information from all phi bins in annulus
	numArray[i] = numTempArray
	fluxArray[i] = fluxTempArray
	timeArray[i] = timeTempArray
	midPhiBin[i] = midPhiTempArray
	fluxRarray[i] = np.sum(fluxTempArray)
	i+=1

#Plotting phi-integrated irradiation profile to screen
pl.plot(midRbin,fluxRarray)
pl.title('Flux v. Radius')
pl.xscale('log')
pl.yscale('log')
pl.show()

#Plotting irradiation map to screen
pl.figure(figsize=(8.,8.))
pl.hist2d(specRadius*np.cos(specPhi), specRadius*np.sin(specPhi), bins = 500, weights = fluxIndiv, norm = LogNorm(), cmap = pl.get_cmap('rainbow'))
pl.colorbar()
pl.xlim([-55.,55.])
pl.ylim([-55.,55.])
pl.title('Flux v. Position')
pl.xlabel(r'x = $\rho$cos($\phi$) [$r_{\rm g}$]')
pl.ylabel(r'y = $\rho$sin($\phi$) [$r_{\rm g}$]')
pl.text(65, 7, r'Incident Flux [Arb]', rotation = 270)
pl.savefig(irrFigFile)
pl.show()


"""
#Plotting travel time map to screen 
pl.figure(figsize=(8.,8.))
pl.hist2d(specRadius*np.cos(specPhi), specRadius*np.sin(specPhi), bins = 500, weights = timeIndiv, norm = LogNorm(), cmap = pl.get_cmap('rainbow'))
pl.colorbar()
pl.xlim([-30.,30.])
pl.ylim([-30.,30.])
pl.title('Flux v. Position')
pl.xlabel(r'x = $\rho$cos($\phi$) [$r_{\rm g}$]')
pl.ylabel(r'y = $\rho$sin($\phi$) [$r_{\rm g}$]')
pl.text(42, 7, r' [Arb]', rotation = 270)
pl.show()
#pl.savefig('./hist2d_time_corona_test.pdf')
"""

#Saving irradiation info to npy file
outFluxArray = [midRbin, midPhiBin, fluxArray, rLimArray, phiLimArray]
outFluxArray = np.array(outFluxArray)
np.save(outFluxFile,outFluxArray)

#Saving time info to npy file
outTimeArray = [midRbin, midPhiBin, timeArray, rLimArray, phiLimArray]
outTimeArray = np.array(outTimeArray)
np.save(outTimeFile,outTimeArray)