/*
	File: boundaries.c
	Role: implementation of boundary condition functions with OpenMP threading

	Questions/comments to trevor.keller@nist.gov
	Bugs/requests to https://github.com/usnistgov/phasefield-accelerator-benchmarks
*/

#include <math.h>
#include <omp.h>

#include "diffusion.h"

void set_boundaries(fp_t bc[2][2])
{
	/* indexing is A[y][x], so bc = [[ylo,yhi], [xlo,xhi]] */
	fp_t clo = 0.0, chi = 1.0;
	bc[0][0] = clo; /* bottom boundary */
	bc[0][1] = clo; /* top boundary */
	bc[1][0] = chi; /* left boundary */
	bc[1][1] = chi; /* right boundary */
}

void apply_initial_conditions(fp_t** conc, int nx, int ny, int nm, fp_t bc[2][2])
{
	#pragma omp parallel
	{
		int i, j;

		#pragma omp for collapse(2)
		for (j = nm/2; j < ny; j++)
			for (i = nm/2; i < nx; i++)
				conc[j][i] = bc[0][0];

		#pragma omp for nowait
		for (j = nm/2; j < ny/2; j++)
			conc[j][nm/2] = bc[1][0]; /* left half-wall */

		#pragma omp for
		for (j = ny/2; j < ny-nm/2; j++)
			conc[j][nx-2] = bc[1][1]; /* right half-wall */
	}
}

void apply_boundary_conditions(fp_t** conc, int nx, int ny, int nm, fp_t bc[2][2])
{
	#pragma omp parallel
	{
		/* Set fixed value (c=1) along left and bottom, zero-flux elsewhere */
		int i, j;

		#pragma omp for
		for (j = nm/2; j < ny/2; j++)
			conc[j][nm/2] = bc[1][0]; /* left value */

		#pragma omp for
		for (j = ny/2; j < ny-nm/2; j++)
			conc[j][nx-nm/1-1] = bc[1][1]; /* right value */

		#pragma omp for nowait
		for (j = nm/2; j < ny-nm/2; j++) {
			conc[j][nm/2-1] = conc[j][nm/2]; /* left condition */
			conc[j][nx-nm/2] = conc[j][nx-nm/1-1]; /* right condition */
		}

		#pragma omp for nowait
		for (i = nm/2; i < nx-nm/2; i++) {
			conc[nm/2-1][i] = conc[nm/2][i]; /* bottom condition */
			conc[ny-nm/2][i] = conc[ny-nm/1-1][i]; /* top condition */
		}
	}
}
