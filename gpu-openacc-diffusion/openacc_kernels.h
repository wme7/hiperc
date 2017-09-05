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
 \file  openacc_kernels.h
 \brief Declaration of functions to execute on the GPU (OpenACC kernels)
*/

/** \cond SuppressGuard */
#ifndef _OPENACC_KERNELS_H_
#define _OPENACC_KERNELS_H_
/** \endcond */

#include "numerics.h"

/**
 \brief Boundary condition kernel for execution on the GPU
*/
void boundary_kernel(fp_t** conc, int nx, int ny, int nm, fp_t bc[2][2]);

/**
 \brief Tiled convolution algorithm for execution on the GPU
*/
void convolution_kernel(fp_t** conc_old, fp_t** conc_lap, fp_t** mask_lap,
                        int nx, int ny, int nm);

/**
 \brief Vector addition algorithm for execution on the GPU
*/
void diffusion_kernel(fp_t** conc_old, fp_t** conc_new, fp_t** conc_lap,
                      int nx, int ny, int nm, fp_t D, fp_t dt);

/**
 \brief Analytical solution comparison algorithm for execution on the GPU
*/
void solution_kernel(fp_t** conc_new, fp_t** conc_lap,  int nx, int ny,
                    fp_t dx, fp_t dy, int nm, fp_t elapsed, fp_t D,
                    fp_t bc[2][2], fp_t* rss);

/** \cond SuppressGuard */
#endif /* _OPENACC_KERNELS_H_ */
/** \endcond */
