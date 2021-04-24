#ifndef ECE486_NCO
#define ECE486_NCO

#define TWO32F 4294967296.0f // Floating point 2^32

typedef struct {
	unsigned int f0_step; // f0 step value for generating samples
	unsigned int theta_step; // Theta step value for generating samples
	float theta; // Track the actual floating point theta value of NCO
	float amp; // Amplitude of the output waveform
	float *lookup; // Pointer to lookup table
	int ctrlWord; // Track position in lookup table

} NCO_T;

NCO_T *init_nco(float f0, float theta);

void nco_get_samples(NCO_T *s, float *y, int n_samples);

void nco_set_frequency(NCO_T *s, float f_new);

void nco_set_phase(NCO_T *s, float theta);

void destroy_nco(NCO_T *s);


#endif
