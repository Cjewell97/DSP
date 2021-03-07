#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ece486_biquad.h"

int main()	
{
	
	int sections = 2;
	float g = 1;

	float biquad_coeffs[] = {1, 1.618, 1, -1.5371, 0.9025, 1, -0.618, 1, 0.0, -0.81};
	int blocksize = 20;

	float x[20];
	float y[20];
	
	x[0] = 3;
	x[1] = 1.5;
	x[2] = -1.8;
	x[3] = 0.2;

	for (int i = 4; i < 20; i++)
	{

		x[i] = 0.0;

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
