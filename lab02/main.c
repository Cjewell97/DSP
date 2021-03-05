#include <stdio.h>
#include <stdlib.h>
#include "ece486_fir.h"

int main(int argc, char **argv){
	float h[] = {1,2,3};
	float x[] = {1,2,3,3,3,3};
	float x1[] = {0,0,0,0,0};
	int blocksize = 6;
	float y[blocksize];
	FIR_T *s = init_fir(h, 3, blocksize);

	calc_fir(s, x, y);
	calc_fir(s, x1,y);
	calc_fir(s, x1,y);
	calc_fir(s, x1,y);

	destroy_fir(s);

}
