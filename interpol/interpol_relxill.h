//One-dimensional linear interpolator (from relutility.c)
double interp_lin_1d(double ifac_r, double rlo, double rhi){
	return ifac_r*rhi + (1.0-ifac_r)*rlo;
}

//Three-dimensional linear interpolator
double interp_lin_3d(double ifrac1, double ifrac2, double ifrac3, double r111, double r211, double r121, double r112, double r221, double r122, double r212, double r222){
	
	//Doing interpolation in x (1st) dimension
	double c11,c12,c21,c22;
	c11 = interp_lin_1d(ifrac1,r111,r211);
	c12 = interp_lin_1d(ifrac1,r112,r212);
	c21 = interp_lin_1d(ifrac1,r121,r221);
	c22 = interp_lin_1d(ifrac1,r122,r222);
	
	//Doing interpolation in y (2nd) dimension
	double c1,c2;
	c1 = interp_lin_1d(ifrac2,c11,c21);
	c2 = interp_lin_1d(ifrac2,c12,c22);
	
	//Doing final interpolation in z (3rd) dimension and then returning the result
	return interp_lin_1d(ifrac3,c1,c2);
	
}