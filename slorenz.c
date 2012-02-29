/*
 * Deterministic Lorenz equations code
 * Research on sensitivity to initial conditions
 *
 * The code receives a set of coordinates of origin and
 * a vector of spherical coordinates in order to cover a
 * sphere of points around the origin.
 *
 * Santiago Nunez Corrales
 * Eric Jakobsson
 * 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <expfile.h>
#include <ranlib.h>

#define R 0
#define PHI 1
#define PSI 2

#define PI 3.141592654

#define SEED1	3
#define SEED2	7

int main (int argc, char *argv[]) {
	int i;
	double curr[DIM];
	double next[DIM];
	double tran[DIM];
	double sphr[DIM];
	double h, a, b, c;
	double sigma, dw, rand;
	FILE *output;
	struct exp_header header;
	
	h = 0.01;
	a = 10.0;
	b = 28.0;
	c = 8.0/3.0;

	if (argc != 9) {
		fprintf(stderr, "Number of parameters incorrect: %d.\n", argc);
		return -1;
	}

	curr[X] = atof(argv[1]);
	curr[Y] = atof(argv[2]);
	curr[Z] = atof(argv[3]);

	sphr[R] = atof(argv[4]);
	sphr[PHI] = atof(argv[5]);
	sphr[PSI] = atof(argv[6]);

	sigma = atof(argv[7]);

	header.count = N;
	memcpy(header.id, "LRNZE", FID*sizeof(char));
	header.x = curr[X];
	header.y = curr[Y];
	header.z = curr[Z];
	header.r = sphr[R];
	header.phi = sphr[PHI];
	header.psi = sphr[PSI];

	sphr[PHI] *= PI/180.0;
	sphr[PSI] *= PI/180.0;

	tran[X] = sphr[R]*cos(sphr[PHI])*sin(sphr[PSI]);
	tran[Y] = sphr[R]*sin(sphr[PHI])*cos(sphr[PSI]);
	tran[Z] = sphr[R]*cos(sphr[PSI]);

	curr[X] += tran[X];
	curr[Y] += tran[Y];
	curr[Z] += tran[Z];

	if ( (output = fopen(argv[8], "wb")) == NULL) {
		fprintf(stderr, "Results file creation failed.\n");
		fclose(output);
		return -1;
	}

	setall(SEED1, SEED2);

	fwrite(&header, 1, sizeof(header), output);
	
	for (i = 0; i < N; i++) {
		rand = gennor(0, 1);
		dw = sqrt(h)*rand;

		next[X] = curr[X] + h*a*(curr[Y] - curr[X]) + sigma*curr[X]*dw;
		next[Y] = curr[Y] + h*(curr[X]*(b - curr[Z]) - curr[Y]) + sigma*curr[Y]*dw;
		next[Z] = curr[Z] + h*(curr[X]*curr[Y] - c*curr[Z]) + sigma*curr[Z]*dw;

		curr[X] = next[X];
		curr[Y] = next[Y];
		curr[Z] = next[Z];

		fwrite(curr, DIM, sizeof(double), output);
	}

	fclose(output);

	return 0;
}

