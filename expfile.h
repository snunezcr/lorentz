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

#define DIM 3
#define FID 5

#define X 0
#define Y 1
#define Z 2

#define N 10000

#define PATH 256

struct exp_header {
	char id[FID];
	int count;
	double x;
	double y;
	double z;
	double xp;
	double yp;
	double zp;
};

struct stat_header {
	char id[FID];
	int count;
	double x;
	double y;
	double z;
};

struct stat_vector {
	double x_avg;
	double x_var;
	double y_avg;
	double y_var;
	double z_avg;
	double z_var;
	double p_avg;
	double p_var;
};

