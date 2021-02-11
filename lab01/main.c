#include <stdio.h>
#include <stdlib.h>
#include "running_mean.h"

int main(int argc, char **argv){
	float x[500] = {0};
	x[0] = 1;

	struct data s = init_running_mean(151, 50);

	s = calc_running_mean(s, x);
	
	free(s.data);
	free(s.mean);

	return 0;
}
