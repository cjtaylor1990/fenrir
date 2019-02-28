import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib as mpl
import csv
import sys

paramList  = 'param_file_converted.txt'
prefix1 = 'fenrir_model_'
prefix2 = 'relxill_model_'
outPrefix = 'refspec_comp_'
path1 = '/Users/cjtaylor/sparse_grid/fenrir_lines/'
path2 = '/Users/cjtaylor/sparse_grid/relxill_lines/'
outPath = '/Users/cjtaylor/sparse_grid/line_overplots/'

table = open(paramList, 'r')
data = csv.reader(table, delimiter = ' ')

aList = []
iList = []

for row in data:
	aList.append(float(row[0]))
	iList.append(float(row[1]))

print aList
print iList
font = {'family' : 'Times New Roman', 'weight' : 'bold', 'size'   : 22}
axes = {'linewidth' : 2.0}
xticksMajor = {'size' : 6.0, 'width' : 2.0}
yticksMajor = {'size' : 6.0, 'width' : 2.0}

mpl.rc('text', usetex = True)
mpl.rc('font', **font)
mpl.rc('axes', **axes)
mpl.rc('xtick.major', **xticksMajor)
mpl.rc('ytick.major', **yticksMajor)
mpl.rc('xtick', direction = 'in')
mpl.rc('ytick', direction = 'in')

pl.figure(figsize=(10.,10.))

index = 0
while (index < len(aList)):
	inFile1 = path1 + prefix1 + str(index+1) + '.npy'
	inFile2 = path2 + prefix2 + str(index+1) + '.npy'

	data1 = np.load(inFile1)
	dataFlux1 = data1[1]
	dataEnergy1 = data1[0]

	data2 = np.load(inFile2)
	dataFlux2 = data2[1]
	dataEnergy2 = data2[0]

	diff = dataFlux1-dataFlux2
	sumSqrDiff = np.sum(diff**2.)

	pl.plot(dataEnergy1,dataFlux1,c='k')
	pl.plot(dataEnergy2,dataFlux2,c='r')
	pl.plot(dataEnergy1,dataFlux1-dataFlux2,c='b')
	#pl.plot(dataEnergy1,dataFlux1/dataFlux2,c='b')
	#pl.xscale('log')
	#pl.yscale('log')
	pl.xlabel(r'$g$')
	pl.ylabel(r'Photon Flux')
	pl.title(r'Line Comparison: a = ' + str(round(aList[index],3)) + ', i = ' + str(round(iList[index],3)) + ', SoSD = ' + str(round(sumSqrDiff,3)))
	pl.savefig(outPath + outPrefix + str(index+1) + '.pdf')
	pl.clf()
	index += 1


