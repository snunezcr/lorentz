/*
 * Deterministic Lorenz equations code
 * Research on sensitivity to initial conditions
 *
 * The code receives a set of coordinates of origin and
 * a vector of spherical coordinates in order to cover a
 * box of points around the origin.
 *
 * Santiago Nunez Corrales
 * Eric Jakobsson
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <expfile.h>
#include <ranlib.h>

#define SEED1	9
#define SEED2	11
#define RAND_ITERS 5

double distance(double xi, double xj, double yi, double yj, double zi, double zj) {
    double dist = 0;
    dist += (xi - xj)*(xi - xj);
    dist += (yi - yj)*(yi - yj);
    dist += (zi - zj)*(zi - zj);
    dist = sqrt(dist);
    return dist;
}

int main (int argc, char *argv[]) {
	int i, j;
    int last_i;
    // Original Lorenz equation result data
    double lorenz[N][DIM];
    // Calculation limits for vector variation
    double strt[DIM];
    double fnsh[DIM];
    double base_x, base_y, base_z;
    // Current element
	double curr[DIM];
	double next[DIM];
	double tran[DIM];
    double points_d, points_s;
    double diff_d, diff_s;
    double resolution;
	double h, a, b, c;
	double sigma, dw, rand;
    // Distance from origin
    double orig_dist;
    double curr_dist;
    // Report differences
    double last_sum_diff;
    double sum_diff;
	FILE *output;
	struct exp_header header;
	
	h = 0.01;
	a = 10.0;
	b = 28.0;
	c = 8.0/3.0;

	if (argc != 8) {
		fprintf(stderr, "Number of parameters incorrect: %d.\n", argc);
        fprintf(stderr, "./%s x y z dr ds r file\n", argv[0]);
		return -1;
	}
    
    // Initialize base execution elements
	base_x = atof(argv[1]);
	base_y = atof(argv[2]);
	base_z = atof(argv[3]);
    points_d = atof(argv[4]);
	points_s = atof(argv[5]);
    resolution = atof(argv[6]);
    diff_d = resolution/points_d;
    diff_s = 1.0/points_s;
    
    // Initialize exploration limits
    strt[X] = base_x - 1;
    strt[Y] = base_y - 1;
    strt[Z] = base_z - 1;
    fnsh[X] = base_x + 1;
    fnsh[Y] = base_y + 1;
    fnsh[Z] = base_z + 1;
    
	if ( (output = fopen(argv[7], "w")) == NULL) {
		fprintf(stderr, "Results file creation failed.\n");
		fclose(output);
		return -1;
	}
    
	setall(SEED1, SEED2);
    
    // Initialize the vector for Lorenz results
    for (i = 0; i < N; i++)
        for (j = 0; j < DIM; j++)
            lorenz[i][j] = 0;
	
    // Step 1: store the standard Lorenz equation
    fprintf(stdout, "Calculating original Lorenz equations at %f, %f, %f.\n",
            base_x, base_y, base_z);
    
    curr[X] = base_x;
    curr[Y] = base_y;
    curr[Z] = base_z;
    
    for (i = 0; i < N; i++) {
        lorenz[i][X] = curr[X];
        lorenz[i][Y] = curr[Y];
        lorenz[i][Z] = curr[Z];
        
        next[X] = curr[X] + h*a*(curr[Y] - curr[X]);
        next[Y] = curr[Y] + h*(curr[X]*(b - curr[Z]) - curr[Y]);
        next[Z] = curr[Z] + h*(curr[X]*curr[Y] - c*curr[Z]);
        
        curr[X] = next[X];
        curr[Y] = next[Y];
        curr[Z] = next[Z];
    }
    
    // Print headers
    fprintf(output, "X0 Y0 Z0 XK YK ZK sigma OD S NL XL YL ZL\n");
    
    // Setp 2: calculate all possible scenarios and their accumulated difference
    
    fprintf(stdout, "Parameter sweep...\n");
    
    for (j = 0; j < RAND_ITERS; j++) {
        fprintf(stdout, "Iteration %i \n", j);
        // Reset for each run
        curr[X] = strt[X];
        curr[Y] = strt[Y];
        curr[Z] = strt[Z];
        
        while (curr[X] <= fnsh[X]) {
            while (curr[Y] <= fnsh[Y]) {
                while (curr[Z] <= fnsh[Z]) {
                    // Calculate distance from origin to current scenario
                    orig_dist = distance(base_x, curr[X], base_y, curr[Y],
                                         base_z, curr[Z]);
                    
		    #pragma omp parallel for
                    for (sigma = 0; sigma <= 1.0; sigma += diff_s) {
                        
                        // Difference starts at 0
                        sum_diff = 0;
                        // Calculation and comparison against Lorenz
                        tran[X] = curr[X];
                        tran[Y] = curr[Y];
                        tran[Z] = curr[Z];
                        
                        
                        for (i = 0; i < N; i++) {
                            last_sum_diff = sum_diff;
                            
                            sum_diff += distance(lorenz[i][X], tran[X],
                                                   lorenz[i][Y], tran[Y],
                                                   lorenz[i][Z], tran[Z]);
                            
                            if(! isfinite(sum_diff)) {
                                sum_diff = last_sum_diff;
                                i = i - 1;
                                break;
                            }
                            
                            rand = gennor(0, 1);
                            dw = sqrt(h)*rand;
                            
                            next[X] = tran[X] 
					+ h*a*(tran[Y] - tran[X]) 
					+ sigma*tran[X]*dw;
                            next[Y] = tran[Y] 
					+ h*(tran[X]*(b - tran[Z]) - tran[Y]) 
					+ sigma*tran[Y]*dw;
                            next[Z] = tran[Z] 
					+ h*(tran[X]*tran[Y] 
					- c*tran[Z]) 
					+ sigma*tran[Z]*dw;

                            tran[X] = next[X];
                            tran[Y] = next[Y];
                            tran[Z] = next[Z];
                            
                        }

                        // Write results
			#pragma omp atomic
                        fprintf(output, "%f %f %f %f %f %f %f %f %.7e %i %.7e %.7e %.7e\n",
                                base_x, base_y, base_z,
                                curr[X], curr[Y], curr[Z],
                                sigma, orig_dist, sum_diff,
                                i, tran[X], tran[Y], tran[Z]);
                    }
                    curr[Z] += diff_d;
                }
                curr[Z] = strt[Z];
                curr[Y] += diff_d;
            }
            curr[Y] = strt[Y];
            curr[X] += diff_d;
        }
    }
    
	fclose(output);

	return 0;
}


