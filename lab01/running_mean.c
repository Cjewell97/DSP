#include <stdio.h>
#include <stdlib.h>
#include "running_mean.h"

struct data init_running_mean(int M, int blocksize){
	struct data mean;
	mean.blocksize = blocksize;
	mean.size = M;
	mean.data = (float*)calloc((M), sizeof(float));
	mean.mean = (float*)calloc((M), sizeof(float));
	mean.rear = M - blocksize + 1;
	return mean;
}

struct data running_queue(struct data mean, float data[]){
	for(int i = 0; i < mean.size; i ++){
		mean.data[i] = data[i]/mean.blocksize;

		mean.mean[i] = mean.mean[(i + mean.size -1)%mean.size] - mean.data[(mean.rear-1+mean.size)%mean.size] + mean.data[i];

		mean.rear = (mean.rear + 1)%mean.size;
		printf("%.3f\n", mean.mean[i]);
	}
	
	printf("\n");
	return mean;
}
