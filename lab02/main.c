#include <stdio.h>
#include <stdlib.h>
#include "ece486_fir.h"
#include "ece486_biquad.h"

int main(int argc, char **argv){

	printf("================ FIR ================\n\n");
	float h[] = {1,2,3};
	float x[] = {1,2,3,3,0,0};
	float x1[] = {0,0,0,0,0};
	int blocksize = 6;
	float y[blocksize];
	FIR_T *s = init_fir(h, 3, blocksize);

	calc_fir(s, x, y);
	calc_fir(s, x1,y);
	calc_fir(s, x1,y);
	calc_fir(s, x1,y);

	destroy_fir(s);
	

	printf("\n\n============== BIQUAD ===============\n\n");
	float bq_coef[] = {1, 1.618,1, -1.5371, 0.9025, 1, -0.618, 1, 0.0, -0.81};
	float x2[20];
	x2[0] = 3;
	x2[1] = 1.5;
	x2[2] = -1.8;
	x2[3] = 0.2;
	float y2[20];

	for (int i = 4; i < 20; i++){
		x2[i] = 0.0;
	}
	BIQUAD_T *ss = init_biquad(2, 1, bq_coef, 20);
	
	calc_biquad(ss, x2, y2);
	
	for(int i = 0; i<20; i++){
		printf("y[%d] = %.3f\n", i,y2[i]);
	}
	destroy_biquad(ss);
}
