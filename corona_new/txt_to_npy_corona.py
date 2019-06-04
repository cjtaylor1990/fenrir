import numpy as np
import matplotlib.pyplot as plt
import csv
import sys

inFile  = str(sys.argv[1])
outFile = str(sys.argv[2])

table = open(inFile, 'r')
data = csv.reader(table, delimiter = ' ')

alpha = []
beta = []
finalEnergy = []
time = []
radius = []
phi = []
theta = []
scaleHeight = []
projectedRadius = []
gamma = []
diskHitSwitch = []

for column in data:
	alpha.append(float(column[0]))
	beta.append(float(column[1]))
	finalEnergy.append(float(column[2]))
	time.append(float(column[3]))
	radius.append(float(column[4]))
	theta.append(float(column[5]))
	phi.append(float(column[6]))
	scaleHeight.append(float(column[7]))
	projectedRadius.append(float(column[8]))
	gamma.append(float(column[9]))
	diskHitSwitch.append(int(column[10]))

alpha = np.array(alpha)
beta = np.array(beta)
finalEnergy = np.array(finalEnergy)
time = np.array(time)
radius = np.array(radius)
phi = np.array(phi)
theta = np.array(theta)
scaleHeight = np.array(scaleHeight)
projectedRadius = np.array(projectedRadius)
gamma = np.array(gamma)
diskHitSwitch = np.array(diskHitSwitch)

outArray = np.array([alpha,beta,finalEnergy,time,radius,theta,phi,scaleHeight,projectedRadius,gamma,diskHitSwitch])

np.save(outFile, outArray)
