import numpy as np
import matplotlib.pyplot as pl
import sys

class Kerr_Metric:

	def __init__(self,spin):
		if (type(spin) in [float,int]) and (np.abs(spin) < 1.0):
			self.a = float(spin)
		else:
			raise ValueError("Spin parameter must be number between -1 and 1 (exclusive).")

	def delta(self, posVec):
		return (posVec[1]**2.) - (2.*posVec[1]) + (self.a**2.)

	def sigma(self, posVec):
		return (posVec[1]**2.) + (self.a**2.)*(np.cos(posVec[2])**2.)

	def gTT(self, posVec):
		return -(1. - (2.*posVec[1]/self.sigma(posVec)))

	def gTPh(self, posVec):
		return -(2.*self.a*posVec[1]*(np.sin(posVec[2])**2.))/self.sigma(posVec)

	def gPhT(self, posVec):
		return gTPh(posVec)

	def gRR(self, posVec):
		return self.sigma(posVec)/self.delta(posVec)

	def gThTh(self, posVec):
		return self.sigma(posVec)

	def gPhPh(self, posVec):
		firstTerm = (posVec[1]**2.) + (self.a**2.)
		secondTerm = (2.*(self.a**2.)*posVec[1]*(np.sin(posVec[2])**2.))/self.sigma(posVec)
		return (firstTerm + secondTerm)*(np.sin(posVec[2])**2.)

	def print_metric(self, posVec):
		print("ds^2 = {}dT^2 + {}dTdPh + {}dR^2 + {}dTh^2 + {}dPh^2".format(self.gTT(posVec),2.*self.gTPh(posVec),
			self.gRR(posVec),self.gThTh(posVec),self.gPhPh(posVec)))

class Disk:

	def __init__(self, metric, thickness):
		self.metric = metric
		self.thickness = thickness

	def rIsco(self):
		a = self.metric.a
		z1 = 1 + (((1-(a*a))**(1./3.))*(((1+a)**(1./3.))+((1-a)**(1./3.))))
		z2 = ((3*(a*a))+(z1*z1))**0.5
		if (a < 0) and (a >= -1):
			rOut = 3+z2+(((3-z1)*(3+z1+(2.*z2)))**0.5)
		elif (a >= 0) and (a <= 1):
			rOut = 3+z2-(((3-z1)*(3+z1+(2.*z2)))**0.5)
		return rOut

	def z(self,projRad):
		if projRad < self.rIsco():
			return 0
		else:
			return self.thickness*(1.-np.sqrt(self.rIsco()/projRad))

	def omega(self,projRad):
		return 1./((projRad**1.5) + self.metric.a)


def calculate_g_factor(metric, disk, posVec, coronaHeight):
	posVecCorona = [0.0, coronaHeight, 0.0, 0.0]
	top = metric.gTT(posVecCorona)
	bottom = metric.gTT(posVec) + (2.*metric.gTPh(posVec)*disk.omega(posVec[1]*np.sin(posVec[2]))) + (metric.gPhPh(posVec)*(disk.omega(posVec[1]*np.sin(posVec[2]))**2.))
	return np.sqrt(-1./bottom)#top/bottom)

def find_posVec(metric, disk, projRad):
	z = disk.z(projRad)
	if z > 0:
		theta = np.arctan(projRad/z)
		radius = projRad/np.sin(theta)
	else:
		theta = np.pi/2.
		radius = projRad
	return [0., radius, theta, 0.0]


#Taking in input parameters
spin = float(sys.argv[1])
height = float(sys.argv[2])
thickness = float(sys.argv[3])
outFile = sys.argv[4]

#Creating the metric and disk instances
metric = Kerr_Metric(spin)
disk = Disk(metric, thickness)

#Setting up radial sample points (cylindrical radii)
rIn = disk.rIsco()
rOut = 100.0
numRad = 1000
sampleRadii = np.linspace(rIn,rOut,numRad+1)

#Finding median of radial bins
medianRadii = (sampleRadii[1:]+sampleRadii[:-1])/2.

#Creating a list of g-factors (for each radius, you find the 4-position vector, which then is used to calculate the g-factor)
gFactorList = [calculate_g_factor(metric,disk,find_posVec(metric, disk, projRad),height) for projRad in medianRadii]

#Plotting g vs. radius
pl.plot(medianRadii, gFactorList)
pl.xscale('log')
pl.yscale('log')
pl.show()

np.save(outFile, np.array([gFactorList,medianRadii]))





	