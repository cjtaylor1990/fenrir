//This is directly copied+pasted from relbase.c. I need to alter it so that it's consistent with 3D interpolation.

// interpolate the table in the A-MU0 plane (for one value of radius)
    static void interpol_a_mu0(int ii, double ifac_a, double ifac_mu0, int ind_a,
    		int ind_mu0, relSysPar* sysPar, relTable* relline_table) {
    	sysPar->gmin[ii] = interp_lin_2d_float(ifac_a, ifac_mu0,
    			relline_table->arr[ind_a][ind_mu0]->gmin[ii], relline_table->arr[ind_a+1][ind_mu0]->gmin[ii],
    			relline_table->arr[ind_a][ind_mu0+1]->gmin[ii], relline_table->arr[ind_a+1][ind_mu0+1]->gmin[ii]);
    
    	sysPar->gmax[ii] = interp_lin_2d_float(ifac_a, ifac_mu0,
    			relline_table->arr[ind_a][ind_mu0]->gmax[ii], relline_table->arr[ind_a+1][ind_mu0]->gmax[ii],
    			relline_table->arr[ind_a][ind_mu0+1]->gmax[ii], relline_table->arr[ind_a+1][ind_mu0+1]->gmax[ii]);
    
    	int jj;
    	for (jj = 0; jj < relline_table->n_g; jj++) {
    		sysPar->trff[ii][jj][0] = interp_lin_2d_float(ifac_a, ifac_mu0,
    				relline_table->arr[ind_a][ind_mu0]->trff1[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0]->trff1[ii][jj],
    				relline_table->arr[ind_a][ind_mu0+1]->trff1[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0+1]->trff1[ii][jj]);
    
    		sysPar->trff[ii][jj][1] = interp_lin_2d_float(ifac_a, ifac_mu0,
    				relline_table->arr[ind_a][ind_mu0]->trff2[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0]->trff2[ii][jj],
    				relline_table->arr[ind_a][ind_mu0+1]->trff2[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0+1]->trff2[ii][jj]);
    
    		sysPar->cosne[ii][jj][0] = interp_lin_2d_float(ifac_a, ifac_mu0,
    				relline_table->arr[ind_a][ind_mu0]->cosne1[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0]->cosne1[ii][jj],
    				relline_table->arr[ind_a][ind_mu0+1]->cosne1[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0+1]->cosne1[ii][jj]);
    
    		sysPar->cosne[ii][jj][1] = interp_lin_2d_float(ifac_a, ifac_mu0,
    				relline_table->arr[ind_a][ind_mu0]->cosne2[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0]->cosne2[ii][jj],
    				relline_table->arr[ind_a][ind_mu0+1]->cosne2[ii][jj],
    				relline_table->arr[ind_a+1][ind_mu0+1]->cosne2[ii][jj]);
    
    	}
    }