#include <stdio.h>
#include <stdlib.h>
#include "ece486_biquad.h"



// Function takes biquad parameters and previous coefficients and creates a new biquad. 
BIQUAD_T * init_biquad(int sections, float g, float * prev_biquads, int blocksize)
{
	printf("Started intializing!\n");
	BIQUAD_T * s = (BIQUAD_T *)malloc(sizeof(BIQUAD_T)); // Create a BIQUAD_T * size memory block
	
	// Set our struct members
	s->sections = sections;
	s->g = g;
	s->blocksize = blocksize;
	s->coefficients = prev_biquads;

	// Create our coefficients arrays. These will be of size sections
	s->v1 = (float *)calloc(sections, sizeof(float));
	s->v2 = (float *)calloc(sections, sizeof(float));

	printf("Finished Initializing!\n");

	// Return our current BIQUAD
	return s;
}

// Calulates the current BIQUADS coefficients
void calc_biquad(BIQUAD_T *s, float * x, float * y)
{
	// Create iterators for the calculation loops
	int section_iter, n;
	
	// Variables to hold current sections coefficients
	float b0, b1, b2;
	float a0, a1, a2;

	
	// Now iterate through each section
	for (section_iter = 0; section_iter < s->sections; section_iter += 6)
	{
		for (n = 0; n < s->blocksize; n++)
		{

			/*	Because the biquad filters are cascaded (multiplied), the gain can be applied
			 *	to the first stage of the filter, and will cascade along with n-1 filters 
			 *	after it. This means we can set our gain on the very first section and
			 *	forget about it, saving time computationally
			 */

			if (section_iter == 0) 
			{
				// Now just multiply each input for x[n] by the gain
				x[n] *= s->g;

			}

			/*	Get our coefficients	
			 *	Formatting of coefficients looks like so:
			 *	[b0, b1, b2, a0, a1, a2]
			 *	To get each coefficient, simply multiply section_iter by 5 to get current 
			 *	coefficients, then add the index of the desired coefficient
			 */

			a0 = s->coefficients[section_iter + 0];
			a1 = s->coefficients[section_iter + 1];
			a2 = s->coefficients[section_iter + 2];

			b0 = s->coefficients[section_iter + 3];
			b1 = s->coefficients[section_iter + 4];
			b2 = s->coefficients[section_iter + 5];


			/*	With the coefficients for current section laid out, 
			 *	begin calculating for x and y
			 */
			
			// y[n] = v1[n-1] + b0x[n]
			y[n] = s->v1[section_iter / 6] + b0 * x[n];

			// Update our v1 and v2 coefficient arrays
			s->v1[section_iter / 6] = s->v2[section_iter / 6] - a1 * y[n] + b1 * x[n];
			s->v2[section_iter / 6] = b2 * x[n] - a2 * y[n];

			// Now set current x[n] to y[n]
			x[n] = y[n];
		}

	}

	return;

}

void destroy_biquad(BIQUAD_T *s)
{

	// Free dynamically allocated memory inside our struct
	free(s->v1);
	free(s->v2);

	// Now free the pointer to the struct
	free(s);

	// All done cleanup
	return;

}

