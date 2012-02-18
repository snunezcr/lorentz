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

#define DIM 3
#define FID 6

#define X 0
#define Y 1
#define Z 2

struct exp_header {
	char id[FID];
	int count;
	double x;
	double y;
	double z;
	double r;
	double phi;
	double psi;
};


