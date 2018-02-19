/**********************************************************************************
 HiPerC: High Performance Computing Strategies for Boundary Value Problems
 written by Trevor Keller and available from https://github.com/usnistgov/hiperc

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
 \file  output.c
 \brief Implementation of file output functions for manufactured solution benchmarks
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include <math.h>
#include <png.h>
#include "numerics.h"
#include "output.h"

void param_parser(int argc,  char* argv[],
                  int*  bx,  int*  by,
                  int* code, int*  steps,
                  fp_t* dx,  fp_t* dy, fp_t* linStab,
                  int*  nx,  int*  ny, int*  nm,
                  fp_t* A1,  fp_t* A2,
                  fp_t* B1,  fp_t* B2,
                  fp_t* C2,  fp_t* kappa)
{
	FILE * input;

	if (argc != 2) {
		printf("Error: improper arguments supplied.\nUsage: ./%s filename\n", argv[0]);
		exit(-1);
	}

	input = fopen(argv[1], "r");
	if (input == NULL) {
		printf("Warning: unable to open parameter file %s. Marching with default values.\n", argv[1]);
	} else {
		char buffer[256];
		char* pch;
		int ibx=0, iby=0, ico=0, idx=0, idy=0, ins=0, inx=0, iny=0, isc=0, ia1=0, ia2=0, ib1=0, ib2=0, ic2=0, ikp=0;

		/* read parameters */
		while ( !feof(input))
		{
			/* process key-value pairs line-by-line */
			if (fgets(buffer, 256, input) != NULL)
			{
				pch = strtok(buffer, " ");

				if (strcmp(pch, "bx") == 0) {
					pch = strtok(NULL, " ");
					*bx = atoi(pch);
					ibx = 1;
				} else if (strcmp(pch, "by") == 0) {
					pch = strtok(NULL, " ");
					*by = atoi(pch);
					iby = 1;
				} else if (strcmp(pch, "co") == 0) {
					pch = strtok(NULL, " ");
					*linStab = atof(pch);
					ico = 1;
				} else if (strcmp(pch, "dx") == 0) {
					pch = strtok(NULL, " ");
					*dx = atof(pch);
					idx = 1;
				} else if (strcmp(pch, "dy") == 0) {
					pch = strtok(NULL, " ");
					*dy = atof(pch);
					idy = 1;
				} else if (strcmp(pch, "ns") == 0) {
					pch = strtok(NULL, " ");
					*steps = atoi(pch);
					ins = 1;
				} else if (strcmp(pch, "nx") == 0) {
					pch = strtok(NULL, " ");
					*nx = atoi(pch);
					inx = 1;
				} else if (strcmp(pch, "ny") == 0) {
					pch = strtok(NULL, " ");
					*ny = atoi(pch);
					iny = 1;
				} else if (strcmp(pch, "sc") == 0) {
					pch = strtok(NULL, " ");
					*nm = atoi(pch);
					pch = strtok(NULL, " ");
					*code = atoi(pch);
					isc = 1;
				} else if (strcmp(pch, "A1") == 0) {
					pch = strtok(NULL, " ");
					*A1 = atof(pch);
					ia1 = 1;
				} else if (strcmp(pch, "A2") == 0) {
					pch = strtok(NULL, " ");
					*A2 = atof(pch);
					ia2 = 1;
				} else if (strcmp(pch, "B1") == 0) {
					pch = strtok(NULL, " ");
					*B1 = M_PI * atof(pch);
					ib1 = 1;
				} else if (strcmp(pch, "B2") == 0) {
					pch = strtok(NULL, " ");
					*B2 = M_PI * atof(pch);
					ib2 = 1;
				} else if (strcmp(pch, "C2") == 0) {
					pch = strtok(NULL, " ");
					*C2 = M_PI * atof(pch);
					ic2 = 1;
				} else if (strcmp(pch, "kp") == 0) {
					pch = strtok(NULL, " ");
					*kappa = atof(pch);
					ikp = 1;
				} else {
					printf("Warning: unknown key %s. Ignoring value.\n", pch);
				}
			}
		}

		/* make sure we got everyone */
		if (! ibx)
			printf("Warning: parameter %s undefined. Using default value, %i.\n", "bx", *bx);
		else if (! iby)
			printf("Warning: parameter %s undefined. Using default value, %i.\n", "by", *by);
		else if (! ico)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "co", *linStab);
		else if (! idx)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "dx", *dx);
		else if (! idy)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "dy", *dy);
		else if (! ins)
			printf("Warning: parameter %s undefined. Using default value, %i.\n", "ns", *steps);
		else if (! inx)
			printf("Warning: parameter %s undefined. Using default value, %i.\n", "nx", *nx);
		else if (! iny)
			printf("Warning: parameter %s undefined. Using default value, %i.\n", "ny", *ny);
		else if (! isc)
			printf("Warning: parameter %s undefined. Using default values, %i and %i.\n", "sc", *nm, *code);
		else if (! ia1)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "A1", *A1);
		else if (! ia2)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "A2", *A2);
		else if (! ib1)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "B1", *B1);
		else if (! ib2)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "B2", *B2);
		else if (! ic2)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "C2", *C2);
		else if (! ikp)
			printf("Warning: parameter %s undefined. Using default value, %f.\n", "kappa", *kappa);
	}
	fclose(input);
}

void print_progress(const int step, const int steps)
{
	static unsigned long tstart;
	time_t rawtime;

	if (step==0) {
		char timestring[256] = {'\0'};
		struct tm* timeinfo;
		tstart = time(NULL);
		time( &rawtime );
		timeinfo = localtime( &rawtime );
		strftime(timestring, 80, "%c", timeinfo);
		printf("%s [", timestring);
		fflush(stdout);
	} else if (step==steps) {
		unsigned long deltat = time(NULL)-tstart;
		printf("•] %2luh:%2lum:%2lus\n",deltat/3600,(deltat%3600)/60,deltat%60);
		fflush(stdout);
	} else if ((20 * step) % steps == 0) {
		printf("• ");
		fflush(stdout);
	}
}

void write_csv(fp_t** conc,
			   const fp_t dx, const fp_t dy,
			   const fp_t elapsed,
			   const int  nx, const int  ny,
			   const int step,
			   const fp_t A1, const fp_t A2,
			   const fp_t B1, const fp_t B2,
			   const fp_t C2, const fp_t kappa)
{
	FILE* output;
	char name[256];
	char num[20];
	int i, j;
	fp_t x, y, s;

	/* generate the filename */
	sprintf(num, "%07i", step);
	strcpy(name, "manufactured.");
	strcat(name, num);
	strcat(name, ".csv");

	/* open the file */
	output = fopen(name, "w");
	if (output == NULL) {
		printf("Error: unable to open %s for output. Check permissions.\n", name);
		exit(-1);
	}

	/* write csv data */
	fprintf(output, "x,y,eta\n");
	for (j = 1; j < ny-1; j++) {
		y = dy * (j - 1);
		for (i = 1; i < nx-1; i++)	{
			x = dx * (i - 1);
			manufactured_solution(x, y, elapsed, A1, A2, B1, B2, C2, kappa, &s);
			fprintf(output, "%f,%f,%f,%f\n", x, y, conc[j][i], s);
		}
	}

	fclose(output);
}

void write_png(fp_t** conc, const int nx, const int ny, const int nm, const int step)
{
	/* After "A simple libpng example program," http://zarb.org/~gc/html/libpng.html
	   and the libpng manual, http://www.libpng.org/pub/png */

	fp_t min, max, *c;
	int i, j, w, h, n;
	FILE* output;
	char name[256];
	char num[20];
	unsigned char* buffer;

	png_infop info_ptr;
	png_bytepp row_pointers;
	png_structp png_ptr;
	png_byte color_type = PNG_COLOR_TYPE_GRAY;
	png_byte bit_depth = 8;

	w = nx - nm + 1;
	h = ny - nm + 1;

	/* generate the filename */
	sprintf(num, "%07i", step);
	strcpy(name, "manufactured.");
	strcat(name, num);
	strcat(name, ".png");

	/* open the file */
	output = fopen(name, "wb");
	if (output == NULL) {
		printf("Error: unable to open %s for output. Check permissions.\n", name);
		exit(-1);
	}

	/* allocate and populate image array */
	buffer = (unsigned char*)malloc(w * h * sizeof(unsigned char));
	row_pointers = (png_bytepp)malloc(h * sizeof(png_bytep));
	for (j = 0; j < h; j++)
		row_pointers[j] = &buffer[w * j];

	/* determine data range */
	min = 0.0;
	max = 1.0;
	for (j = nm/2; j < ny - nm/2; j++) {
		for (i = nm/2; i < nx - nm/2; i++) {
			c = &conc[j][i];
			if (*c < min)
				min = *c;
			if (*c > max)
				max = *c;
		}
	}

	/* printf("Data spans [%.2e, %.2e]\n", min, max); */

	/* rescale data into buffer */
	n = 0;
	for (j = ny - nm/2 - 1; j > nm/2 - 1; j--) {
		for (i = nm/2; i < nx - nm/2; i++) {
			buffer[n] = (unsigned char) 255 * (min + (conc[j][i] - min) / (max - min));
			n++;
		}
	}
	if (n != w * h) {
		printf("Error making image: expected %i values in buffer, got %i.\n", w*h, n);
		exit(-1);
	}

	/* let libpng do the heavy lifting */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		printf("Error making image: png_create_write_struct failed.\n");
		exit(-1);
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("Error making image: unable to init_io.\n");
		exit(-1);
	}
	png_init_io(png_ptr, output);

	/* write PNG header */
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("Error making image: unable to write header.\n");
		exit(-1);
	}
	png_set_IHDR(png_ptr, info_ptr, w, h,
	                 bit_depth, color_type, PNG_INTERLACE_NONE,
	                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	/* write image */
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("Error making image: unable to write data.\n");
		exit(-1);
	}
	png_write_image(png_ptr, row_pointers);

	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("Error making image: unable to finish writing.\n");
		exit(-1);
	}
	png_write_end(png_ptr, NULL);

	/* clean up */
	fclose(output);
	free(row_pointers);
	free(buffer);
}
