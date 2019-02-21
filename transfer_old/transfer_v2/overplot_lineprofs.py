import numpy as np
import numpy.ma as mask
import matplotlib.pyplot as pl
import matplotlib as mpl
import csv
import sys

filePath = './'
#outFile = '/Users/cjtaylor/Documents/conferences/aas2018/linecomp_a09.png'

inFile1 = filePath + 'construct_lineprof.npy'#'#myrazor_test11_newscript.npy'#'corona_a0998_acc0_r10_onaxis_n1000_tol500_hist_test.npy'
inFile2 = filePath + 'transfer_test.npy'#'./pvc_cart_n1000_spec.npy'#./relline_test9+10+11_newmod.npy'
#inFile3 = filePath + 'spec_a09_h3_i15_mdot02_rout30.npy'#'#myrazor_test11_newscript.npy'#'corona_a0998_acc0_r10_onaxis_n1000_tol500_hist_test.npy'
#inFile4 = filePath + 'spec_a09_h3_i15_mdot03_rout30.npy'#'./pvc_cart_n1000_spec.npy'#./relline_test9+10+11_newmod.npy'
outFile = '/Users/cjtaylor/Desktop/transfer_test.pdf'

data1 = np.load(inFile1)
dataFlux1 = data1[1]
dataEnergy1 = data1[0]

data2 = np.load(inFile2)
dataFlux2 = data2[1]
dataEnergy2 = data2[0]

"""
dEdata1 = np.empty(len(dataEnergy1)-1)
dEdata2 = np.empty(len(dataEnergy2)-1)
dEdata3 = np.empty(len(dataEnergy3)-1)
dEdata4 = np.empty(len(dataEnergy4)-1)

dataSum1 = 0.
dataSum2 = 0.
dataSum3 = 0.
dataSum4 = 0.

j = 0
while (j < len(dataEnergy1)-1):
	dEdata[j] = dataEnergy1[j+1]-dataEnergy1[j]
	dataSum += dataFlux1[j]*dEdata[j]
	j+=1

plotDataEnergy = dataEnergy1[:-1]
plotDataFlux = dataFlux1[:-1]
plotModelEnergy = modelEnergy1[:-1]
plotModelFlux = modelFlux1[:-1]

#dataSum = np.sum(plotDataFlux)
#modelSum = np.sum(plotModelFlux)

print plotDataEnergy[0]
print plotModelEnergy[0]

sigma = 5.*10.**-2.
eCenter = 6.4
gau = np.exp(-0.5*((plotDataEnergy-eCenter)/(5.*10.**-2.))**2.)
conv = np.convolve((plotDataFlux/dataSum),gau,mode='full')
convX = np.arange(len(conv))
dE = (plotModelEnergy[-1]-plotModelEnergy[0])/len(plotModelEnergy)
dX = (convX[-1]-convX[0])/len(convX)

print dE
print dX
print convX[-1]-convX[0]
print len(convX)
"""
font = {'family' : 'Times New Roman', 'weight' : 'bold', 'size'   : 26}
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
#pl.plot(convX*(dE/2.),conv/np.sum(conv)*(dX))
pl.plot(dataEnergy1, dataFlux1/np.sum(dataFlux1), color = 'k', linestyle = '-')#(plotDataFlux)/dataSum
pl.plot(dataEnergy2, dataFlux2/np.sum(dataFlux2), color = 'r', linestyle = '-')
#pl.plot(dataEnergy1, (dataFlux1/np.sum(dataFlux1))-(dataFlux2/np.sum(dataFlux2)), color = 'b', linestyle = '-')
#pl.plot(dataEnergy3, dataFlux3/np.max(dataFlux1), color = 'r', linestyle = '-')#(plotDataFlux)/dataSum
#pl.plot(dataEnergy4, dataFlux4/np.max(dataFlux1), color = 'b', linestyle = '-')
#pl.plot(plotDataEnergy, ((plotDataFlux)/dataSum)-((plotModelFlux)/modelSum))
#pl.plot(indexarray, modelEnergy1, color = 'r', marker='o')
#pl.xscale('log')
#pl.yscale('log')
pl.xlabel(r'$g$')
pl.ylabel(r'Normalized Photon Flux')
#pl.ylabel(r'F/g')
#pl.xlim([-0.25,1.3])
#pl.ylim([0.0,1.2])
pl.savefig(outFile)
pl.show()

