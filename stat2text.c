/*
 * Deterministic Lorenz equations code
 * Research on sensitivity to initial conditions
 *
 * Converts a statistics file from binary to ASCII formats.
 *
 * Santiago Nunez Corrales
 * Eric Jakobsson
 * 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <expfile.h>

int main(int argc, char *argv[]) {
	struct stat_header header;
	struct stat_vector vect;
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

	if (strncmp(header.id, "LRNZS", FID) != 0) {
		fprintf(stderr, "Not an experiment file.\n");
		fclose(out);
		return -1;
	}

	for (i = 0; i < header.count; i++) {
		error = fread(&vect, 1, sizeof(vect), in);
		fprintf(out, "%d\t", i);
		fprintf(out, "%lf\t", vect.x_avg);
		fprintf(out, "%lf\t", vect.x_var);
		fprintf(out, "%lf\t", vect.y_avg);
		fprintf(out, "%lf\t", vect.y_var);
		fprintf(out, "%lf\t", vect.z_avg);
		fprintf(out, "%lf\t", vect.z_var);
		fprintf(out, "%lf\t", vect.p_avg);
		fprintf(out, "%lf\t", vect.p_var);
                fprintf(out, "\n");
	}

	fclose(out);
	fclose(in);

	return 0;
}
