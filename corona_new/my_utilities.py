
import csv

def read_txt_file(inFile,numVals = 1,delimiter = ' ',isFloat=True):
	"""This function reads in the given text file and gives its contents back as a list
	of lists, each sublist a different column of the original text file.

	Example:
	If example.txt is:
	1 2
	3 4

	Then,
	read_txt_file('example.txt',numVals = 2, delimiter = ' ', isFloat=True)
	returns
	[[1,3],[2,4]]

	numVals = number of columns (default 1)
	delimiter = delimiter of columns (default ' ')
	isFloat = tells function if you want it in float (True)
		or string (False) format (default True)
	"""
	table = open(inFile, 'r')
	data = csv.reader(table, delimiter = delimiter)

	outList = []
	i = 0
	while (i < numVals):
		outList.append([])
		i+=1

	for row in data:
		i = 0
		while (i < numVals):
			if isFloat==True:
				item = float(row[i])
			else:
				pass
			outList[i].append(item)
			i+=1

	return outList

def rIsco(a):
	"""Calculates the inner-most stable circular orbit (or rIsco, in gravitational
	radii Rg) for a given dimensionless spin value. |a| <= 1

	Example: rIsco(0) => 6.0 """
	if type(a) is not float or a > 1.0 or a < -1.0:
		raise ValueError("Spin parameter must be a float in range [-1.0,1.0], inclusive.")
	else:
		z1 = 1 + (((1-(a*a))**(1./3.))*(((1+a)**(1./3.))+((1-a)**(1./3.))))
		z2 = ((3*(a*a))+(z1*z1))**0.5
		if (a < 0) and (a >= -1):
			rOut = 3+z2+(((3-z1)*(3+z1+(2.*z2)))**0.5)
		elif (a >= 0) and (a <= 1):
			rOut = 3+z2-(((3-z1)*(3+z1+(2.*z2)))**0.5)
		return rOut

def rEvent(spin):
	"""Calculates the radius of the event horizon given an input spin value. The value
	must be a float between -1 and 1. Spin here is the typical dimensionless spin
	parameter.

	rEvent(0.0) => 2.0"""
	if type(a) is not float or a > 1.0 or a < -1.0:
		raise ValueError("Spin parameter must be a float in range [-1.0,1.0], inclusive.")
	else:
		return 1+np.sqrt(1-spin**2)