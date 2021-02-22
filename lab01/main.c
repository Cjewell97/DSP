#include <stdio.h>
#include <stdlib.h>
#include "running_mean.h"

int main(int argc, char **argv){
	float x[] = {0,1,2,3,4,5,6,7};

	struct data s = init_running_mean(4, 6);

	s = calc_running_mean(s, x);
	s = calc_running_mean(s, x);
	s = calc_running_mean(s, x);
	
	free(s.data);
	free(s.mean);

	return 0;
}
