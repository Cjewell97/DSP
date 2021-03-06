#ifndef ECE_BIQUAD_H
#define ECE_BIQUAD_H

#include <stdio.h>
#include <stdlib.h>

/*	Biquad Structure Prototype	*/
typedef struct biquad
{
	int sections;
	float g;
	int blocksize;
	
	/* Biquad has a and b coefficients, as well as its previous output for n sections before  */
	float * coefficients;
	float * v1;
	float * v2;
} BIQUAD_T;

/* Function Prototypes */
BIQUAD_T *init_biquad(int sections, float g, float * biquad_coeffs, int blocksize);
void calc_biquad(BIQUAD_T *s, float * x, float * y);
void destroy_biquad(BIQUAD_T * s);

#endif
