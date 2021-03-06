#include <stdio.h>
#include <stdlib.h>

typedef struct biquad {
	int sections;
	float g;
	int blocksize;
	float* coef;
	float v1;
	float v2;
} BIQUAD_T;

BIQUAD_T* init_biquad(int sections, float g, float* biquad_coeffs, int blocksize);
void calc_biquad(BIQUAD_T *s, float *x, float *y);
void destroy_biquad(BIQUAD_T *s);
