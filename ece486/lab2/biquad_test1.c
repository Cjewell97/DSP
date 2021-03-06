#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece486_biquad.h"

int main() 
{

	int sections = 1;
	float g = 1;

	float biquad_coeffs[] = {1, -0.1712407, 0.1767567, 0.251379, 0.5027580, 0.25137900};
	int blocksize = 20;

	float x[20];
	float y[20];

	for (int i = 0; i < sizeof(x); i++)
	{
		if (i == 0) {
			x[i] = 1;
		}
		else {
			x[i] = 0;
		}
	}
	
	BIQUAD_T *s = init_biquad(sections, g, biquad_coeffs, blocksize);
	
	calc_biquad(s, x, y);
	for (int n = 0; n < s->blocksize; n++)
	{
		printf("%d: %f\n", n, y[n]);
	}

	destroy_biquad(s);

	return 0;



}
