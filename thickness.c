#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>

// l is span
// x is ?
// sigma is stress
double get_thickness(double l, double x, double dp, double tau_xy, double wg_1, double wg_2, double wgs_1a, double wgs_2a, double wgs_1b, double wgs_2b, double theta_1y, double theta_2y, double theta_1a, double theta_2a, double theta_1b, double theta_2b,  double limit, double sigma_x_init, double sigma_y_init){
	double sigma_x, sigma_x_local, sigma_x_local_prev, sigma_x_global;
	double sigma_y, sigma_y_local, sigma_y_local_prev, sigma_y_global;
	double Mx, My, My1, My2;
	double sigma_v, sigma_v_prev;
	double thickness=7, thickness_prev=7, thickness_x, thickness_y;
	
	const double E= 200000;
	const double p= 0.1029;
	const double L= 143.75;
	const double y= 71.875;
	const num_mesh_x= 6;
	const num_mesh_y= 24;
	
	
	Mx= E * pow(thickness,3) / 12 *( \
			+ wg_1 * (-6/pow(l,2) + 12*x/pow(l,3))
			+ theta_1y * (4/l -  6*x/pow(l,2))
			+ wg_2 * (6/pow(l,2)-12*x/pow(l,3))
			+ theta_2y * (2/l - 6*x/pow(l,2)) );
	sigma_x_global= 6 * Mx  /  pow(thickness,2);
	
	My1 =  -E * pow(thickness,3) / 12 *( \
			+ wgs_1a * (-6/pow(L,2) + 12*y/pow(L,3))
			+ theta_1a * (4/L -  6*y/pow(L,2))
			+ wgs_2a * (6/pow(L,2)-12*y/pow(L,3))
			+ theta_2a * (2/L - 6*y/pow(L,2)) );
	My2 =  -E * pow(thickness,3) / 12 *( \
			+ wgs_1b * (-6/pow(L,2) + 12*y/pow(L,3))
			+ theta_1b * (4/L -  6*y/pow(L,2))
			+ wgs_2b * (6/pow(L,2)-12*y/pow(L,3))
			+ theta_2b * (2/L - 6*y/pow(L,2)) );
	My = My1 + ( l / (double)num_mesh_x * (double)dp - 0.0) * (My2 - My1) / l;
	sigma_y_global= 6 * My / pow(thickness,2);
	
	sigma_x_local= sigma_x_init - sigma_x_global;
	sigma_y_local= sigma_y_init - sigma_y_global;
		
	do{
		thickness_prev= thickness;
		sigma_x_local_prev= sigma_x_local;
		sigma_x_local= pow(thickness_prev,2) /pow(thickness,2) *sigma_x_local_prev;
		
		Mx= E * pow(thickness,3) / 12 *( \
			+ wg_1 * (-6/pow(l,2) + 12*x/pow(l,3))
			+ theta_1y * (4/l -  6*x/pow(l,2))
			+ wg_2 * (6/pow(l,2)-12*x/pow(l,3))
			+ theta_2y * (2/l - 6*x/pow(l,2)) );
		sigma_x_global= 6 * Mx  /  pow(thickness,2);
	
		sigma_y_local_prev= sigma_y_local;
		sigma_y_local= pow(thickness_prev,2) /pow(thickness,2) *sigma_y_local_prev;
		
		My1 =  -E * pow(thickness,3) / 12 *( \
			+ wgs_1a * (-6/pow(L,2) + 12*y/pow(L,3))
			+ theta_1a * (4/L -  6*y/pow(L,2))
			+ wgs_2a * (6/pow(L,2)-12*y/pow(L,3))
			+ theta_2a * (2/L - 6*y/pow(L,2)) );
		My2 =  -E * pow(thickness,3) / 12 *( \
			+ wgs_1b * (-6/pow(L,2) + 12*y/pow(L,3))
			+ theta_1b * (4/L -  6*y/pow(L,2))
			+ wgs_2b * (6/pow(L,2)-12*y/pow(L,3))
			+ theta_2b * (2/L - 6*y/pow(L,2)) );
		My = My1 + ( l / (double)num_mesh_x * (double)dp - 0.0) * (My2 - My1) / l;
		sigma_y_global= 6 * My / pow(thickness,2);
	
		sigma_x= sigma_x_local + sigma_x_global;
		sigma_y= sigma_y_local + sigma_y_global;
		sigma_v_prev= sigma_v;
		sigma_v=sqrt( 0.5 * (pow(sigma_x-sigma_y,2) + pow(sigma_x,2) + pow(sigma_y,2) + 6*pow(tau_xy,2) ));
		
		// for l is constant
		thickness_x= sqrt( sigma_x_local_prev/sigma_x_local * pow(thickness_prev,2));
		thickness_y= sqrt( sigma_y_local_prev/sigma_y_local * pow(thickness_prev,2));
		thickness= (thickness_x>thickness_y)?thickness_x:thickness_y;
		}
	while (sigma_v<=limit); // limit said to be 235
	
	// last known thickness is good
	return (thickness_prev);
}

int main(int argc, char **argv){
	}
