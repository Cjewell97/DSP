#include <stdio.h>
#include <stdlib.h>
#include "ece486_fir.h"

/*
filt : initialize memory filter structure
filt->old : initialize array to store the number of needed values
filt->h : store filter coefficients of FIR
filt->n_coef : number of coefficients in h
filt->blocksize : store size of input block in struct
filt->marker: keep marker to track index of filt->old[]
*/
FIR_T *init_fir(float *fir_coefs, int n_coef, int blocksize){
	FIR_T *filt;
	filt = (FIR_T*)malloc(sizeof(FIR_T));
	filt->old = (float*)calloc(n_coef, sizeof(float));
	filt->h = fir_coefs;
	filt->n_coef = n_coef;
	filt->blocksize = blocksize;
	filt->marker = 0;
	return filt;
}

/*
calculate the output for FIR using a discrete time convolution
inputs:
 - s : filter struct
 - x : input data block
 - y : output data block
*/
void calc_fir(FIR_T *s, float *x, float *y){

// loop through all elements of input block
	for(int i = 0; i < s->blocksize; i++){
		y[i] = 0;

// loop through all values for convolution summation
		for(int n = 0; n < s->n_coef; n++){

			// if we need to index previous saved values of x, use s->old[] array
			// index s->old[] using the marker
			if(i >= n){
				y[i] += s->h[n] * x[i-n];
			}else{
				y[i] += s->h[n] * s->old[(s->marker + i - n + s->n_coef)%s->n_coef];
			}
		}
		printf("y[%d] = %.3f\n", i, y[i]);
	}
	for(int n = 0; n < s->blocksize; n++){
		//  set the old values of x[n]
		s->old[(s->marker + n)%s->n_coef] = x[n];
	}
	// increment the marker
	s->marker = (s->marker + s->blocksize)%s->n_coef;
}

// function to free memory
void destroy_fir(FIR_T *s){
	free(s->old);	
	free(s);
};
