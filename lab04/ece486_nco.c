/****************************************************************


 file: ece486_nco.c
 Description:  Subroutines to implement numerically controlled oscillators
        Multiple subroutine calls are used to create sample sequences
        for sinusoidal functions with programmable frequency and phase.
            y(n) = cos(2 pi f0 n + theta)
        

 Interface:
     init_nco() is called once (for each oscillator), and is used to create
                any required oscillator structures.  This routine handles all
                array initialization, allocates any required memory,
                and perform any other required initialization.
     nco_set_frequency() is used to change the frequency of the nco 
     nco_set_phase() is used to change the phase of the nco 
     nco_get_samples() return a block of nco output samples.
     destroy_nco() is called once at the end of the program, and is used
                  to de-allocate any memory associated with an nco.

  Function Prototypes and parameters:

        #include "nco.h"
        NCO_T *init_nco(float f0, float theta);
           Inputs:
                f0     Initial NCO discrete-time frequency for the output sample
                       sequence (in cycles/sample)
                theta  Inital Phase of the output sample sequence (radians)
           Returned:
                The function returns a pointer to a "NCO_T" data type
                (which is defined in "nco.h")
        
        void nco_get_samples(NCO_T *s, float *y, int n_samples);
           Inputs:
              s         pointer to initialized NCO_T
              y         Caller-supplied array to be replaced by NCO output samples
              n_samples Number of NCO output samples to generate.
           Returned:
              On return, y[i] (for i=0,1,...n_samples-1) contains the the next
              n_samples values of the NCO output sequence.  Subsequent calls to 
              nco_get_samples() continues to return NCO output samples with no 
              phase discontinuities from one call to the next. 
              Example:
                NCO_T *s;
                float y1[15],y2[5];
                s=init_nco(0.11, 0.0);       // set up NCO for .11 cycles/sample
                nco_get_samples(s, y1, 15);  // Generate samples 0,1, ..., 14 of y(n)
                nco_get_samples(s, y2, 5);   // Generate samples 15,16,17,18,19 of y(n)
              
        void nco_set_frequency(NCO_T *s, float f_new);
          Inputs:
             s         pointer to NCO_T
             f_new     New NCO frequency (in cycles/sample).
          Returned:
             The NCO_T structure s is modified so that subsequent calls to nco_get_samples()
             will operate at frequency f_new (without loss of phase continuity).
        
        void nco_set_phase(NCO_T *s, float theta);
          Inputs:
             s         pointer to NCO_T
             theta     New NCO phase.
          Returned:
             The NCO_T structure s is modified so that subsequent calls to nco_get_samples()
             will operate with the phase shift given by theta (in radians)
        
        void destroy_nco(NCO_T *s);
           Inputs:
                s       pointer to NCO_T, as provided by init_nco()
           Returned: Any resources associated with the nco "s" are released.
 
 
*******************************************************************/ 

#include "ece486_nco.h"

#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"

#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "arm_math.h"

NCO_T *init_nco(float f0, float theta){
    /*******  ECE486 STUDENTS MODIFY THIS *******/
	// Allocate memory to store the new NCO struct
	NCO_T *s = (NCO_T *) malloc(sizeof(NCO_T));
	int k;

	// If the memory allocation fails
	if (s == NULL) 
	{
		printf("Failed to allocate memory!\n");
		flagerror(MEMORY_ALLOCATION_ERROR);
		return NULL;
	}


	// Set attributes of NCO_T struct
	s->center_freq_step = f0 * pow(2, 32);
	s->individual_theta_step = theta * pow(2, 32) / (2 * M_PI);
	s->ctrlWord = s->individual_theta_step;
	s->amplitude = 0.9;
	s->theta = theta;

	// Now create space for the lookup table
	s->table = (float *)calloc(512, sizeof(float));

	// If memory allocation fails
	if (s->table == NULL) 
	{
		printf("Failed to allocate memory!\n");
		flagerror(MEMORY_ALLOCATION_ERROR);
		return NULL;
	}

	for (k = 0; k < 512; k++) 
	{
		s->table[k] = cos(2 * M_PI / 512);
	}

	return s;

}

void nco_get_samples(NCO_T *s, float *y, int n_samples){
    /*******  ECE486 STUDENTS MODIFY THIS *******/
	int iter;

	// For each in n_samples, index the lookup table, and get the appropriate output
	for(iter = 0; iter < n_samples; iter++) 
	{
		s->ctrlWord += s->center_freq_step;

		y[iter] = s->amplitude * s->table[(s->ctrlWord >> 23) & 0x1FF];
	}
}

void nco_set_frequency(NCO_T *s, float f_new){
    /*******  ECE486 STUDENTS MODIFY THIS *******/
	s->center_freq_step = f_new * MAX_32_VALUE;	
}

void nco_set_phase(NCO_T *s, float theta){
    /*******  ECE486 STUDENTS MODIFY THIS *******/

	// Set single theta step
	s->individual_theta_step = theta * MAX_32_VALUE / (2 * M_PI);

	// Save our theta to the structure
	s->theta = theta;

	// Save our single step/phase to the struct
	s->ctrlWord += s->individual_theta_step;

}

void destroy_nco(NCO_T *s){
    /*******  ECE486 STUDENTS MODIFY THIS *******/
	// Get rid of the lookup table
	if (s->table != NULL) 
	{
		free(s->table);
	}

	// Then destroy the object

	if (s != NULL) 
	{
		free(s);
	}

}
