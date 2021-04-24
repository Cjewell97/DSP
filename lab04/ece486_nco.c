#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include "ece486_nco.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "arm_math.h"
#include <stdint.h>

NCO_T *init_nco(float f0, float theta){
	// Allocate memory for  NCO_T struct
	NCO_T *s = (NCO_T *)malloc(sizeof(NCO_T));

	// Check for allocation errors
	if (s == NULL) {
		printf("Failed to allocate memory for NCO_T *s\n");
		flagerror(MEMORY_ALLOCATION_ERROR);
		while(1);
		return NULL;
	}

	// Set attributes of NCO_T struct based on inputs
	s->f0_step = f0*pow(2, 32); // Caculate index step based on target freq
	s->theta_step = theta*pow(2,32)/(2*M_PI); // Step size for phase shift
	s->ctrlWord = s->theta_step; // Offset index in array by phase shift
	s->amp = 0.9; // Amplitude < 1 to not overflow DAC
	s->theta = theta; // Keep track of theta value
	
	// Allocate memory for lookup table
	s->lookup = (float *)calloc(512, sizeof(float));

	// Check for errors
	if (s->lookup == NULL) {
		printf("Failed to allocate memory for lookup table\n");
		flagerror(MEMORY_ALLOCATION_ERROR);
		while(1);
		return NULL;
	}

	// Generate lookup table values for 1 cycle of cosine at freq f0
	int k;
	for (k = 0; k < 512; k++) {
		s->lookup[k] = cos(2*M_PI*k/512);
	}

	// Return created NCO_T struct
	return s;

}

// Return n samples of the NCO function from lookup table
void nco_get_samples(NCO_T *s, float *y, int n_samples){

	int k;
	for (k=0; k < n_samples; k++) {
		// Phase accumulator, increment by frequency step size
		s->ctrlWord += s->f0_step;

		// Get sample value based on lookup table and scaled by amp
		y[k] = s->amp * s->lookup[(s->ctrlWord>>23)&0x1FF];
	}
}

// Set the new frequency of NCO by recalculating the new frequency step size
void nco_set_frequency(NCO_T *s, float f_new){
	// Set the f0 val in NCO_T struct to f_new
	s->f0_step = f_new*TWO32F;
}

// Set a new phase for the NCO by offsetting the current index
void nco_set_phase(NCO_T *s, float theta){
	// Calculate a new theta_step value for index offset
	s->theta_step = theta*TWO32F/(2*M_PI);

	// Track the floating point value of the phase shift
	s->theta = theta;

	// Apply index offset due to phase shift to the index into the table
	s->ctrlWord += s->theta_step;
}


void destroy_nco(NCO_T *s){
	// Free memory inside NCO_T struct
	if (s->lookup != NULL) {
		free(s->lookup);
	}

	// Free the NCO_T struct
	if (s != NULL) {
		free(s);
	}
}
