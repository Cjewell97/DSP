#include <stdio.h>
#include <stdlib.h>
#include "ece486_biquad.h"

BIQUAD_T* init_biquad(int sections, float g, float* prev_biquads, int blocksize){
	BIQUAD_T* s = (BIQUAD_T*)malloc(sizeof(BIQUAD_T));
	
	s->sections = sections;
	s->g = g;
	s->blocksize = blocksize;
	s->coef = prev_biquads;	
	s->v1 = 0;
	s->v2 = 0;

	return s;
}

void calc_biquad(BIQUAD_T *s, float *x, float *y){
	for(int sec = 0; sec < s->sections; sec++){
		for(int n = 0; n < s->blocksize; n++){
			
			if(sec == 0){
				x[n] *= s->g;
			}
			
			// y[n] = v1[n - 1] + b0 * x[n]
			y[n] = s->v1 + s->coef[5*sec] * x[n];
			printf("y[%d] = %.3f\n", n, y[n]);
			// v1[n] = v2[n - 1] - a1 * y[n] + b1 * x[n]
			s->v1 = s->v2 - s->coef[5*sec + 3] * y[n] + s->coef[5*sec + 1] * x[n];
			// v2[n] = b2 * x[n] - a2 * y[n]
			s->v2 = s->coef[5*sec + 2] - s->coef[5*sec + 4] * y[n];
		}
	}
}

void destroy_biquad(BIQUAD_T *s){
	free(s);
}
