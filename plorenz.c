/*
 * Stochastic Lorenz equations code
 * Research on sensitivity to initial conditions
 *
 * The code receives a set of coordinates of origin and
 * a vector of spherical coordinates in order to cover a
 * sphere of points around the origin. This code allows relaxing
 * several elements at once through noise. In particular, having
 * noisy parameters.
 *
 * Santiago Nunez Corrales
 * Eric Jakobsson
 * 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <expfile.h>
#include <ranlib.h>

#define SEED1	3
#define SEED2	7

int main (int argc, char *argv[]) {
	int i;
	double curr[DIM];
	double next[DIM];
	double tran[DIM];
	double h, a, b, c;
	double sigma, omega, dw, rand, rand_a, rand_b, rand_c;
	FILE *output;
	struct exp_header header;

	h = 0.01;
	a = 10.0;
	b = 28.0;
	c = 8.0/3.0;

	if (argc != 10) {
		fprintf(stderr, "Number of parameters incorrect: %d.\n", argc);
		return -1;
	}

	curr[X] = atof(argv[1]);
	curr[Y] = atof(argv[2]);
	curr[Z] = atof(argv[3]);

	tran[X] = atof(argv[4]);
	tran[Y] = atof(argv[5]);
	tran[Z] = atof(argv[6]);

	sigma = atof(argv[7]);

	omega = atof(argv[8]);

	curr[X] += tran[X];
	curr[Y] += tran[Y];
	curr[Z] += tran[Z];

	if ( (output = fopen(argv[9], "wb")) == NULL) {
		fprintf(stderr, "Results file creation failed.\n");
		fclose(output);
		return -1;
	}

	setall(SEED1, SEED2);

	for (i = 0; i < N; i++) {
		rand = gennor(0, 1);
		dw = sqrt(h)*rand;

		// Compute stochastic parameters using omega
		rand_a = gennor(a, a*omega);
		rand_b = gennor(b, b*omega);
		rand_c = gennor(c, c*omega);

		// Compute next coordinates from current ones
		next[X] = curr[X] + h*rand_a*(curr[Y] - curr[X]) + sigma*curr[X]*dw;
		next[Y] = curr[Y] + h*(curr[X]*(rand_b - curr[Z]) - curr[Y]) + sigma*curr[Y]*dw;
		next[Z] = curr[Z] + h*(curr[X]*curr[Y] - rand_c*curr[Z]) + sigma*curr[Z]*dw;

		// Update current ones
		curr[X] = next[X];
		curr[Y] = next[Y];
		curr[Z] = next[Z];

		// Instead of a binary file, write a standard CSV file
		// fwrite(curr, DIM, sizeof(double), output);
		fprintf(output, "%i,%.6e,%.6e,%.6e\n", i, curr[X], curr[Y], curr[Z]);
	}
	fclose(output);

	return 0;
}
