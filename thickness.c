#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>

double get_thickness(double l, double L, double x, double tau_xy, double wg_1, double wg_2, double wgs_1, double wgs_2, double theta_1x, double theta_1y, double theta_2x, double theta_2y, double limit, double sigma_x_init, double sigma_y_init){
	double sigma_x, sigma_x_local, sigma_x_local_prev, sigma_x_global;
	double sigma_y, sigma_y_local, sigma_y_local_prev, sigma_y_global;
	double mx, my;
	double sigma_v, sigma_v_prev;
	double thickness=7, thickness_prev=7, thickness_x, thickness_y;
	
	const double E= 200000;
	const double p= 0.1029;
	
	mx= E * pow(thickness,3) / 12 *( \
			+ wg_1 * (-6/pow(l,2) + 12*x/pow(l,3))
			+ theta_1y * (4/l -  6*x/pow(l,2))
			+ wg_2 * (6/pow(l,2)-12*x/pow(l,3))
			+ theta_2y * (2/l - 6*x/pow(l,2)) );
	sigma_x_global= 6 * mx  /  pow(thickness,2);
	my= -E * pow(thickness,3) / 12 *( \
		+ wgs_1 * (-6/pow(L,2) + 12*x/pow(L,3))
		+ theta_1x * (4/L -  6*x/pow(L,2))
		+ wgs_2 * (6/pow(L,2)-12*x/pow(L,3))
		+ theta_2x * (2/L - 6*x/pow(L,2)) );
	sigma_y_global= 6 * my / pow(thickness,2);
	sigma_x_local= sigma_x_init - sigma_x_global;
	sigma_y_local= sigma_y_init - sigma_y_global;
		
	do{
		thickness_prev= thickness;
		sigma_x_local_prev= sigma_x_local;
		sigma_x_local= pow(thickness_prev,2) /pow(thickness,2) *sigma_x_local_prev;
		
		mx= E * pow(thickness,3) / 12 *( \
			+ wg_1 * (-6/pow(l,2) + 12*x/pow(l,3))
			+ theta_1y * (4/l -  6*x/pow(l,2))
			+ wg_2 * (6/pow(l,2)-12*x/pow(l,3))
			+ theta_2y * (2/l - 6*x/pow(l,2)) );
		sigma_x_global= 6 * mx  /  pow(thickness,2);
	
		sigma_y_local_prev= sigma_y_local;
		sigma_y_local= pow(thickness_prev,2) /pow(thickness,2) *sigma_y_local_prev;
		
		my= -E * pow(thickness,3) / 12 *( \
			+ wgs_1 * (-6/pow(L,2) + 12*x/pow(L,3))
			+ theta_1x * (4/L -  6*x/pow(L,2))
			+ wgs_2 * (6/pow(L,2)-12*x/pow(L,3))
			+ theta_2x * (2/L - 6*x/pow(L,2)) );
		sigma_y_global= 6 * my / pow(thickness,2);
	
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
	printf("%d",get_thickness(20, 30, 40, 0.3, 4.98, 5.1, 4.9, 5.1, 4.9, 5.01, 9.69e-4, 3.04e-16, 235, 20, 20));
	}
