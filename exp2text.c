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
#include <expfile.h>

int main(int argc, char *argv[]) {
	struct exp_header header;
	double vect[DIM];
	int i;
	int error;

	FILE *in;
	FILE *out;

	if (argc != 3) {
		fprintf(stderr, "Number of parameters incorrect: %d.\n", argc);
		return -1;
	}

	if ( (in = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Data file open failed.\n");
		fclose(in);
		return -1;
        }

	if ( (out = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "Text file creation failed.\n");
		fclose(out);
		return -1;
	}

	error = fread(&header, 1, sizeof(header), in);

	if (strncmp(header.id, "LRNZE", FID - 1) != 0) {
		fprintf(stderr, "Not an experiment file.\n");
		fclose(out);
		return -1;
	}

	for (i = 0; i < header.count; i++) {
		error = fread(vect, DIM, sizeof(double), in);
		fprintf(out, "%d\t%lf\t%lf\t%lf\n", i, vect[X], vect[Y], vect[Z]);
	}

	fclose(out);
	fclose(in);

	return 0;
}
