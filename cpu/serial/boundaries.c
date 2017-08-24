/**********************************************************************************
 This file is part of Phase-field Accelerator Benchmarks, written by Trevor Keller
 and available from https://github.com/usnistgov/phasefield-accelerator-benchmarks.

 This software was developed at the National Institute of Standards and Technology
 by employees of the Federal Government in the course of their official duties.
 Pursuant to title 17 section 105 of the United States Code this software is not
 subject to copyright protection and is in the public domain. NIST assumes no
 responsibility whatsoever for the use of this software by other parties, and makes
 no guarantees, expressed or implied, about its quality, reliability, or any other
 characteristic. We would appreciate acknowledgement if the software is used.

 This software can be redistributed and/or modified freely provided that any
 derivative works bear some notice that they are derived from it, and any modified
 versions bear some notice that they have been modified.

 Questions/comments to Trevor Keller (trevor.keller@nist.gov)
 **********************************************************************************/

/**
 \file  cpu/serial/boundaries.c
 \brief Implementation of boundary condition functions without threading
*/

#include <math.h>

#include "diffusion.h"

/**
 \brief Set values to be used along the simulation domain boundaries

 Indexing is row-major, i.e. A[y][x], so bc = [[ylo,yhi], [xlo,xhi]].
*/
void set_boundaries(fp_t bc[2][2])
{
	fp_t clo = 0.0, chi = 1.0;
	bc[0][0] = clo; /* bottom boundary */
	bc[0][1] = clo; /* top boundary */
	bc[1][0] = chi; /* left boundary */
	bc[1][1] = chi; /* right boundary */
}

/**
 \brief Initialize flat composition field with fixed boundary conditions

 The boundary conditions are fixed values of 1 along the lower-left half and
 upper-right half walls, no flux everywhere else, with an initial value of 0
 everywhere. These conditions represent a carburizing process, with partial
 exposure (rather than the entire left and right walls) to produce an
 inhomogeneous workload and highlight numerical errors at the boundaries.
*/
void apply_initial_conditions(fp_t** conc, int nx, int ny, int nm, fp_t bc[2][2])
{
	int i, j;

	for (j = 0; j < ny; j++)
		for (i = 0; i < nx; i++)
			conc[j][i] = bc[0][0];

	for (j = 0; j < ny/2; j++)
		for (i = 0; i < 1+nm/2; i++)
			conc[j][i] = bc[1][0]; /* left half-wall */

	for (j = ny/2; j < ny; j++)
		for (i = nx-1-nm/2; i < nx; i++)
			conc[j][i] = bc[1][1]; /* right half-wall */
}

/**
 \brief Set fixed value (c=1) along left and bottom, zero-flux elsewhere
*/
void apply_boundary_conditions(fp_t** conc, int nx, int ny, int nm, fp_t bc[2][2])
{
	int i, j;

	for (j = 0; j < ny/2; j++)
		for (i = 0; i < 1+nm/2; i++)
			conc[j][i] = bc[1][0]; /* left value */

	for (j = ny/2; j < ny; j++)
		for (i = nx-1-nm/2; i < nx; i++)
			conc[j][i] = bc[1][1]; /* right value */

	for (j = 0; j < ny; j++) {
		for (i = nm/2; i > 0; i--)
			conc[j][i-1] = conc[j][i]; /* left condition */
		for (i = nx-1-nm/2; i < nx-1; i++)
			conc[j][i+1] = conc[j][i]; /* right condition */
	}

	for (i = 0; i < nx; i++) {
		for (j = nm/2; j > 0; j--)
			conc[j-1][i] = conc[j][i]; /* bottom condition */
		for (j = ny-1-nm/2; j < ny-1; j++)
			conc[j+1][i] = conc[j][i]; /* top condition */
	}
}
