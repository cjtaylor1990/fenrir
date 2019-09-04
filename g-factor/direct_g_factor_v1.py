import numpy as np
import matplotlib.pyplot as pl
import scipy.stats as stats
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

#Taking input data from command file (input, output, spin)
inFile = sys.argv[1]
outFile = sys.argv[2]
a = float(sys.argv[3])

#Defining radial sample bin based on the input spin (which determines the inner radius)
rIn = rIsco(a)
rOut = 100.0
numRad = 1000
sampleRadii = np.linspace(rIn,rOut,numRad+1)

#Calculating the median of bins
medianRadii = (sampleRadii[1:]+sampleRadii[:-1])/2.

#Loading in corona data
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

#Finding photons that fall within the min and max radii
#iRcut = np.where(np.logical_and((np.logical_and(np.logical_and(projectedRadius > rIn,projectedRadius < rOut), diskHitSwitch > 0)),(gRatio < 100.)))
iRcut = np.where((np.logical_and(np.logical_and(projectedRadius > rIn,projectedRadius < rOut), diskHitSwitch > 0)))

#Finding the energy (g-factor) and projected radius of those photons that hit the disk
specEnergy = (gRatio)[iRcut]
specRadius = (projectedRadius)[iRcut]

#Using scipy.stats (stats) to bin the photons by radius and calulate the mean (returns a collection of arrys, the 0-indexed array being the array of means)
meanEnergy = stats.binned_statistic(specRadius, specEnergy, 'mean', sampleRadii)

#Plotting the mean energy as a function of the radius
pl.plot(medianRadii, meanEnergy[0], color = 'k')
pl.axvline(rIn, color = 'g')
pl.yscale('log')
pl.xscale('log')
pl.xlim([1.,100.])
pl.show()

np.save(outFile, np.array([meanEnergy, medianRadii]))
