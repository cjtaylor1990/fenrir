
double linear_interpol(double p1, double p2, double f1, double f2, double p){
	double pd = (p-p1)/(p2-p1);
	return ((1-pd)*f1) + (pd*f2);
}

double trilinear_interpol(double xSample[2], double ySample[2], double zSample[2], double (*func)(double,double,double), double interpolPos[3]){
	
	//Deconstructing sample arrays
	double x0,x1,y0,y1,z0,z1;
	x0 = xSample[0];
	x1 = xSample[1];
	y0 = ySample[0];
	y1 = ySample[1];
	z0 = zSample[0];
	z1 = zSample[1];

	//Deconstructing interpolated vector
	double x,y,z;
	x = interpolPos[0];
	y = interpolPos[1];
	z = interpolPos[2];
	
	//Doing interpolation in x (0-th) dimension
	double c00,c01,c10,c11;
	c00 = linear_interpol(x0,x1,func(x0,y0,z0),func(x1,y0,z0),x);
	c01 = linear_interpol(x0,x1,func(x0,y0,z1),func(x1,y0,z1),x);
	c10 = linear_interpol(x0,x1,func(x0,y1,z0),func(x1,y1,z0),x);
	c11 = linear_interpol(x0,x1,func(x0,y1,z1),func(x1,y1,z1),x);
	
	//Doing interpolation in y (1-th) dimension
	double c0,c1;
	c0 = linear_interpol(y0,y1,c00,c10,y);
	c1 = linear_interpol(y0,y1,c01,c11,y);
	
	//Doing final interpolation in z (2-th) dimension and then returning the result
	return linear_interpol(z0,z1,c0,c1,z);
	
}