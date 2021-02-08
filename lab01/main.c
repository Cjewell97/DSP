#include <stdio.h>
#include <stdlib.h>
#include "running_mean.h"

int main(int argc, char **argv){
	float M[] = {0,1,2,3,4,5,6,7};
	float N[] = {8,9,10,11,12,13,14,15};
	float O[] = {0,0,0,1,2,3,4,5};
	struct data s = init_running_mean(8, 3);

	running_queue(s, M);
	running_queue(s, M);
	free(s.data);
	free(s.mean);

	return 0;
}
