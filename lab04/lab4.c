#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ece486_nco.h"
#include <stdint.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include <stdint.h>

// To detect a button press
extern FlagStatus KeyPressed;

#define DISPLAY_CLEAR	(uint8_t) "        "
#define SAMPLING_FREQUENCY	32000.00000
#define ONE_KHZ	1000.0

int main(void){

	int nsamp; // Size of input and output blocks
	int i, j; // Iteration variables
	float *input, *output1, *output2; // Arrays for input and output blocks
	int counter = 0; // Track number of input blocks
	int button_count = 0; // Track number of button presses
	int F_0 = 0; // Center frequency for the mixer
	uint16_t numTaps = 300; // Number of coefficients for FIR filter
	uint8_t M = 10; // Decimation factor
	uint32_t blockSize; // Size of input and output blocks
	uint32_t fftSize = 1024; // Set size for ARM FFT

	// Create ARM FIR filter instances
	arm_fir_decimate_instance_f32 s;
	arm_fir_decimate_instance_f32 s3;

	// Set the blocksize to 1280, will make the fftsize equal to 1024
	setblocksize(1280);

	// Initialize the board
	initialize_ece486(FS_32K, MONO_IN, STEREO_OUT, HSE_EXTERNAL_8MHz);

	// Keep track of blocksize
	nsamp = getblocksize(); 
	blockSize = nsamp;

	// Memory Allocation
	input = (float *)malloc(sizeof(float)*nsamp);
	output1 = (float *)malloc(sizeof(float)*nsamp);
	output2 = (float *)malloc(sizeof(float)*nsamp);

	// Keep track of decimated samples
	float32_t *decBlock1 = (float32_t *)calloc(nsamp/M, sizeof(float32_t));
	float32_t *decBlock2 = (float32_t *)calloc(nsamp/M, sizeof(float32_t));

	// Keep track of FIR filter states between blocks
	float32_t *pState = (float32_t *)calloc(numTaps+blockSize-1,sizeof(float32_t));
	float32_t *pState2 = (float32_t *)calloc(numTaps+blockSize-1,sizeof(float32_t));

	// Check for memory allocation errors
	if (input==NULL||output1==NULL||output2==NULL) {
		flagerror(MEMORY_ALLOCATION_ERROR);
		while(1);
	}

	// Set up NCO waveforms
	NCO_T *s1 = init_nco((ONE_KHZ)/(SAMPLING_FREQUENCY), 0.0);
	NCO_T *s2 = init_nco((ONE_KHZ)/(SAMPLING_FREQUENCY), 3*M_PI/2);

	// Change the power to prevent the DAC from blowing up
	s1->amplitude = 0.9;
	s2->amplitude = 0.9;

	// Coefficients for FIR lowpass decimation filter found from MATLAB (FilterDesigner.m)
	float32_t pCoeffs[] = {
		1.49796e-10, -4.35172e-10, -1.60607e-09, -4.08764e-09, -8.52118e-09, 
		-1.55834e-08, -2.58131e-08, -3.93781e-08, -5.57879e-08, -7.3586e-08, 
		-9.00797e-08, -1.01185e-07, -1.0147e-07, -8.44919e-08, -4.34924e-08, 
		2.75152e-08, 1.32293e-07, 2.70677e-07, 4.36813e-07, 6.17631e-07, 
		7.91899e-07, 9.30258e-07, 9.96597e-07, 9.51053e-07, 7.54732e-07, 
		3.7602e-07, -2.01956e-07, -9.73487e-07, -1.90313e-06, -2.9206e-06, 
		-3.91902e-06, -4.75806e-06, -5.27279e-06, -5.28887e-06, -4.64375e-06, 
		-3.21246e-06, -9.3601e-07, 2.1513e-06, 5.89937e-06, 1.00294e-05, 
		1.41344e-05, 1.76976e-05, 2.01297e-05, 2.08257e-05, 1.92388e-05, 
		1.49641e-05, 7.8269e-06, -2.03917e-06, -1.41331e-05, -2.75607e-05, 
		-4.10527e-05, -5.30334e-05, -6.17445e-05, -6.54178e-05, -6.24884e-05, 
		-5.18287e-05, -3.29783e-05, -6.34087e-06, 2.66822e-05, 6.36531e-05, 
		0.000101175, 0.000135102, 0.000160875, 0.000173979, 0.000170477, 
		0.000147575, 0.000104165, 4.12594e-05, -3.77364e-05, -0.000126988, 
		-0.000218464, -0.000302476, -0.000368484, -0.000406153, -0.000406553, 
		-0.000363402, -0.000274199, -0.000141123, 2.8467e-05, 0.000222034, 
		0.000422449, 0.000609187, 0.000760076, 0.000853499, 0.000870858, 
		0.000799051, 0.000632712, 0.000375899, 4.29892e-05, -0.000341442, 
		-0.000743875, -0.00112425, -0.0014394, -0.00164732, -0.00171177, 
		-0.00160701, -0.00132183, -0.000862666, -0.000255157, 0.000456184, 
		0.00121032, 0.00193412, 0.00254857, 0.00297643, 0.00315053, 
		0.00302202, 0.0025677, 0.00179541, 0.000746852, -0.000502756, 
		-0.00184865, -0.00316395, -0.00431014, -0.00515006, -0.00556206, 
		-0.00545414, -0.00477673, -0.00353242, -0.00178156, 0.000357307, 
		0.00271282, 0.00507195, 0.00719643, 0.00884343, 0.00978885, 
		0.00985121, 0.00891409, 0.00694472, 0.00400673, 0.000265402, 
		-0.00401571, -0.00848719, -0.0127348, -0.0163077, -0.0187528, 
		-0.0196509, -0.0186541, -0.0155186, -0.0101325, -0.00253428, 
		0.00707983, 0.0183599, 0.0308186, 0.0438593, 0.056813, 
		0.068982, 0.0796879, 0.0883187, 0.0943719, 0.097491, 
		0.097491, 0.0943719, 0.0883187, 0.0796879, 0.068982, 
		0.056813, 0.0438593, 0.0308186, 0.0183599, 0.00707983, 
		-0.00253428, -0.0101325, -0.0155186, -0.0186541, -0.0196509, 
		-0.0187528, -0.0163077, -0.0127348, -0.00848719, -0.00401571, 
		0.000265402, 0.00400673, 0.00694472, 0.00891409, 0.00985121, 
		0.00978885, 0.00884343, 0.00719643, 0.00507195, 0.00271282, 
		0.000357307, -0.00178156, -0.00353242, -0.00477673, -0.00545414, 
		-0.00556206, -0.00515006, -0.00431014, -0.00316395, -0.00184865, 
		-0.000502756, 0.000746852, 0.00179541, 0.0025677, 0.00302202, 
		0.00315053, 0.00297643, 0.00254857, 0.00193412, 0.00121032, 
		0.000456184, -0.000255157, -0.000862666, -0.00132183, -0.00160701, 
		-0.00171177, -0.00164732, -0.0014394, -0.00112425, -0.000743875, 
		-0.000341442, 4.29892e-05, 0.000375899, 0.000632712, 0.000799051, 
		0.000870858, 0.000853499, 0.000760076, 0.000609187, 0.000422449, 
		0.000222034, 2.8467e-05, -0.000141123, -0.000274199, -0.000363402, 
		-0.000406553, -0.000406153, -0.000368484, -0.000302476, -0.000218464, 
		-0.000126988, -3.77364e-05, 4.12594e-05, 0.000104165, 0.000147575, 
		0.000170477, 0.000173979, 0.000160875, 0.000135102, 0.000101175, 
		6.36531e-05, 2.66822e-05, -6.34087e-06, -3.29783e-05, -5.18287e-05, 
		-6.24884e-05, -6.54178e-05, -6.17445e-05, -5.30334e-05, -4.10527e-05, 
		-2.75607e-05, -1.41331e-05, -2.03917e-06, 7.8269e-06, 1.49641e-05, 
		1.92388e-05, 2.08257e-05, 2.01297e-05, 1.76976e-05, 1.41344e-05, 
		1.00294e-05, 5.89937e-06, 2.1513e-06, -9.3601e-07, -3.21246e-06, 
		-4.64375e-06, -5.28887e-06, -5.27279e-06, -4.75806e-06, -3.91902e-06, 
		-2.9206e-06, -1.90313e-06, -9.73487e-07, -2.01956e-07, 3.7602e-07, 
		7.54732e-07, 9.51053e-07, 9.96597e-07, 9.30258e-07, 7.91899e-07, 
		6.17631e-07, 4.36813e-07, 2.70677e-07, 1.32293e-07, 2.75152e-08, 
		-4.34924e-08, -8.44919e-08, -1.0147e-07, -1.01185e-07, -9.00797e-08, 
		-7.3586e-08, -5.57879e-08, -3.93781e-08, -2.58131e-08, -1.55834e-08, 
		-8.52118e-09, -4.08764e-09, -1.60607e-09, -4.35172e-10, 1.49796e-10
	};

	// Initialization for CMSIS DSP ARM FIR Decimation filter	
	arm_fir_decimate_init_f32(&s, numTaps, M, pCoeffs, pState, blockSize);
	arm_fir_decimate_init_f32(&s3, numTaps, M, pCoeffs, pState2, blockSize);

	// Allocate FFT arrays
	float32_t *fft_array = (float32_t *)malloc(2*fftSize*sizeof(float32_t));
	float32_t *fft_output = (float32_t *)malloc(fftSize*sizeof(float32_t));

	// Check for allocation errors
	if (fft_array == NULL || fft_output == NULL) {
		flagerror(MEMORY_ALLOCATION_ERROR);
		while(1);
	} 

	// List of window coefficients generated by Matlab Kaiser.m script
	float32_t win[] = {

		0.0199357, 0.0277415, 0.0367597, 0.0470511, 0.05867, 
		0.0716625, 0.0860663, 0.101909, 0.119209, 0.137973, 
		0.158196, 0.179862, 0.202941, 0.227392, 0.253161, 
		0.28018, 0.308371, 0.33764, 0.367884, 0.398987, 
		0.430822, 0.463254, 0.496135, 0.529311, 0.562621, 
		0.595897, 0.628966, 0.661653, 0.693779, 0.725164, 
		0.755631, 0.785004, 0.81311, 0.839781, 0.864857, 
		0.888184, 0.909619, 0.929029, 0.946291, 0.961297, 
		0.973952, 0.984175, 0.9919, 0.997078, 0.999675, 
		0.999675, 0.997078, 0.9919, 0.984175, 0.973952, 
		0.961297, 0.946291, 0.929029, 0.909619, 0.888184, 
		0.864857, 0.839781, 0.81311, 0.785004, 0.755631, 
		0.725164, 0.693779, 0.661653, 0.628966, 0.595897, 
		0.562621, 0.529311, 0.496135, 0.463254, 0.430822, 
		0.398987, 0.367884, 0.33764, 0.308371, 0.28018, 
		0.253161, 0.227392, 0.202941, 0.179862, 0.158196, 
		0.137973, 0.119209, 0.101909, 0.0860663, 0.0716625, 
		0.05867, 0.0470511, 0.0367597, 0.0277415, 0.0199357, 
	};

	// Windowing scale factor, found using Kaiser.m Matlab script
	float W0 = 1017.9;

	printf("Starting execution using %d samples per input block.\n",nsamp);


	while(1){
		/*
		 * Ask for a block of ADC samples to be put into the working buffer
		 *   getblock() will wait here until the input buffer is filled...  On return
		 *   we work on the new data buffer... then return here to wait for
		 *   the next block
		 */
		getblock(input);

		counter++; // Keep track of current block number

		/*
		 * signal processing code to calculate the required output buffers
		 */

		DIGITAL_IO_SET();  // Use a scope on PD0 to measure execution time

		// Set the current center frequency for spectrum analysis
		nco_set_frequency(s1, F_0/SAMPLING_FREQUENCY);
		nco_set_frequency(s2, F_0/SAMPLING_FREQUENCY);

		// Now analyze that center frequency range
		nco_get_samples(s1, output1, nsamp);
		nco_get_samples(s2, output2, nsamp);

		// Invert output2 to generate other side of spectrum (Negative frequencies)
		for (i = 0; i < nsamp; i++) {
			output1[i] = input[i] * output1[i];
			output2[i] = -input[i] * output2[i];
		}

		// Send input through FIR Decimation filter
		arm_fir_decimate_f32(&s, output1, decBlock1, blockSize);
		arm_fir_decimate_f32(&s3, output2, decBlock2, blockSize);

		// Put alternating samples from filter output real,imag into fft array
		for (i = 0; i < nsamp/M ; i++) { 
			// decBlock len = nsamp/M
			fft_array[(counter-1)*2*nsamp/M+(i*2)] = decBlock1[i]*win[(counter-1)*nsamp/M+i];
			fft_array[(counter-1)*2*nsamp/M+(i*2)+1] = decBlock2[i]*win[(counter-1)*nsamp/M+i];
		}

		// Once 1024 decimated samples are collected then do FFT
		if (counter > 7) {
			// CALCULATE FFT IN HERE
			arm_cfft_f32( &arm_cfft_sR_f32_len1024, fft_array, 0, 1); // Calculate FFT
			arm_cmplx_mag_f32(fft_array, fft_output, fftSize); // Get magnitude of vectors
			arm_scale_f32(fft_output, 4/(W0), fft_output, fftSize); // Scale for output to DAC

			counter = 0; // Reset block counter
		}

		// Put FFT results to output
		for (i = 0; i < nsamp ; i++) {

			if ((i > (fftSize-192)) || (i < 192)) {
				// Mask out frequencies out of range
				output2[i] = -1.0;
			}

			else if (i < 512) {
				// Show -1kHz -> 0
				output2[i] = fft_output[1023-(511-i)]-1.0;

			} 
			else if (i >= 512) {
				// Show 0 -> +1kHz
				output2[i] = fft_output[i-512]-1.0;

			}

			// Generate triangle wave for reference
			if (i < 192) {
				// Mask out frequencies out of range
				output1[i] = -1.0;

			}

			else if (i > (fftSize-192)) {
				// Mask out frequencies out of range
				output1[i] = -1.0;

			} 
			else {
				// Line equation using magnitude of frequency step
				output1[i] = (i*(2.0/fftSize)-1.0);  
			}

		}


		DIGITAL_IO_RESET();	// (falling edge....  done processing data )

		/*
		 * pass the processed working buffer back for DAC output
		 */
		putblockstereo(output1, output2);


		/* Set current frequency center, and display it (Takes care of question 3) */

		// Cycle through center frequency of mixer, f0, by pushing the button
		if (KeyPressed) {
			KeyPressed = RESET;
			button_count++;
		}
		// If button presses exceeds the upper center limit, restart back to 0 
		if (button_count > 15) {
			button_count = 0;
		}

		// Multiply button count by 1000 to get KHz stepped frequency center
		F_0 = button_count*1000;

		// String to hold F_0
		char dispString[8];

		// Display f_0
		sprintf(dispString,"%d",F_0);

		// Clear the display
		BSP_LCD_GLASS_DisplayString(DISPLAY_CLEAR);

		// Display the center frequency
		BSP_LCD_GLASS_DisplayString((uint8_t *)dispString);


	}
}
