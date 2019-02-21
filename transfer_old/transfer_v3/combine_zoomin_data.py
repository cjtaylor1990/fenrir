import numpy as np
import matplotlib.pyplot as plt
import csv
import sys

inFile  = str(sys.argv[1])
outFile = str(sys.argv[2])
imgSize = float(sys.argv[3])
maxImgSize = float(sys.argv[4])

correction = (imgSize/maxImgSize)**2.

table = open(inFile, 'r')
data = csv.reader(table, delimiter = ' ')

x = []
y = []
gRatio = []
time = []
radius = []
phi = []
theta = []
scaleHeight = []
projectedRadius = []

for column in data:
	x.append(float(column[0]))
	y.append(float(column[1]))
	gRatio.append(float(column[2]))
	time.append(float(column[3]))
	radius.append(float(column[4]))
	theta.append(float(column[5]))
	phi.append(float(column[6]))
	scaleHeight.append(float(column[7]))
	projectedRadius.append(float(column[8]))

x = np.array(x)
y = np.array(y)
gRatio = np.array(gRatio)    
time = np.array(time)
radius = np.array(radius)
phi = np.array(phi)
theta = np.array(theta)
scaleHeight = np.array(scaleHeight)
projectedRadius = np.array(projectedRadius)

correctArray = np.zeros(len(x))
correctArray[:] = correction

outArray = np.array([x,y,gRatio,time,radius,theta,phi,scaleHeight,projectedRadius,correctArray])

np.save(outFile, outArray)
