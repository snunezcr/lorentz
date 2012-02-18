/*
 * Deterministic Lorenz equations code
 * Research on sensitivity to initial conditions
 *
 * Generates statistics from a set of experiments.
 *
 * Santiago Nunez Corrales
 * Eric Jakobsson
 * 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <expfile.h>

double norm(double x, double y, double z) {
	return sqrt(x*x + y*y + z*z);
}

void init_stat(struct stat_vector *data, int n) {
	int i;
	
	for (i = 0; i < n; i++) {
		data[i].x_avg = 0;
		data[i].x_var = 0;
		data[i].y_avg = 0;
		data[i].y_var = 0;
		data[i].z_avg = 0;
		data[i].z_var = 0;
		data[i].p_avg = 0;
		data[i].p_var = 0;
	}
}

int process_file(char *file, struct stat_vector *data, 
						struct stat_header *st_hd) {
	struct exp_header header;
	double vec[DIM];
	double p_norm;
	int i;
	int error;
	FILE *in;

	if ( (in = fopen(file, "rb")) == NULL) {
		fprintf(stderr, "Data file open failed: %s\n", file);
		fclose(in);
		return 0;
        }

	 error = fread(&header, 1, sizeof(header), in);

	if (strncmp(header.id, "LRNZE", FID - 1) != 0) {
                fprintf(stderr, "Not an experiment file.\n");
		fclose(in);
		return 0;
	}

	/* Set values for the first time in the header */
	if (st_hd->count < 0) {
		st_hd->x = header.x;
		st_hd->y = header.y;
		st_hd->z = header.z;
		st_hd->count = header.count;
	}

	printf("Processing file: %s\n", file);

	for (i = 0; i < header.count; i++) {
		error = fread(vec, DIM, sizeof(double), in);
		p_norm = norm(vec[X], vec[Y], vec[Z]);
		data[i].x_avg += vec[X];
		data[i].x_var += vec[X]*vec[X];
		data[i].y_avg += vec[Y];
		data[i].y_var += vec[Y]*vec[Y];
		data[i].z_avg += vec[Z];
		data[i].z_var += vec[Z]*vec[Z];
		data[i].p_avg += p_norm;
                data[i].p_var += p_norm*p_norm;
	}

	fclose(in);
	return 1;
}

void post_compute(struct stat_vector *data, int k, int n) {
	int i;

	/* Normalize against k */
	for (i = 0; i < n; i++) {
		data[i].x_avg *= 1.0/k;
		data[i].x_var *= 1.0/k;
		data[i].y_avg *= 1.0/k;
		data[i].y_var *= 1.0/k;
		data[i].z_avg *= 1.0/k;
		data[i].z_var *= 1.0/k;
		data[i].p_avg *= 1.0/k;
		data[i].p_var *= 1.0/k;
	}

	/* Calculate the variance for each point */
	for (i = 0; i < n; i++) {
		data[i].x_var -= data[i].x_avg*data[i].x_avg;
		data[i].y_var -= data[i].y_avg*data[i].y_avg;
		data[i].z_var -= data[i].z_avg*data[i].z_avg;
		data[i].p_var -= data[i].p_avg*data[i].p_avg;
	}
}

int main (int argc, char *argv[]) {
	DIR *directory;
	struct dirent *dp;
	struct stat_header header;
	struct stat_vector *data;
	char file_path[PATH];
	FILE *out;
	int error;
	int k;

	if (argc != 3) {
		fprintf(stderr, "Number of parameters incorrect: %d.\n", argc);
		return -1;
	}

	k = 0;

	/* Signal that data has not been filled out for the header */
	header.count = -1;
	memcpy(header.id, "LRNZS", FID*sizeof(char));

	data = (struct stat_vector *) malloc(N*sizeof(struct stat_vector));
	init_stat(data, N);
	
	if ((directory = opendir(argv[1])) == NULL) {
                fprintf(stderr, "Unable to open directory: %s", argv[1]);
                return -1;
        }

	do {
		if ((dp = readdir(directory)) != NULL) {
			if (strcmp(dp->d_name, ".") == 0 ||
				strcmp(dp->d_name, "..") == 0)
				continue;

			strcpy(file_path, argv[1]);
			strcat(file_path, "/");
			strcat(file_path, dp->d_name);

			if (process_file(file_path, data, &header))
				k++;
		}
	} while (dp != NULL);

	closedir(directory);

	if (k == 0) {
		fprintf(stderr, "No valid files processed.\n");
		return -1;
	}

	post_compute(data, k, N);

	if ( (out = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "Text file creation failed.\n");
		fclose(out);
		return -1;
        }

	fwrite(&header, 1, sizeof(header), out);
	fwrite(data, N, sizeof(struct stat_vector), out);
	fclose(out);

	free(data);
	return 0;
}

