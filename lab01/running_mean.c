#include <stdio.h>
#include <stdlib.h>
#include "running_mean.h"
/*
Initialize the struct:
 - set the blocksize and size of average
 - allocate memory to track the past M values of the data
 - allocate memory for output
All memory is initialized to zero

create a tracker (mean.front) to keep track of the start of the queue; keep track of the first data point being averaged.
*/
struct data init_running_mean(int M, int blocksize){
	struct data mean;
	mean.blocksize = blocksize;
	mean.size = M;
	mean.data = (float*)calloc((M), sizeof(float));
	mean.mean = (float*)calloc((blocksize), sizeof(float));
	mean.front = 0;
	return mean;
}

/*
Calculate the running mean
*/

struct data calc_running_mean(struct data mean, float data[]){
	for(int i = 0; i < mean.blocksize; i++){

/*
To calculate the mean, add the previous value of the mean and the current data point divided by the mean size
Subtract the last data point that is not being used in the mean anymore.
*/
		mean.mean[i] = mean.mean[(i + mean.blocksize - 1)%mean.blocksize] + data[i]/mean.size - mean.data[(mean.front + mean.size)%mean.size];

// save the input data into the mean.data array for future interations
	mean.data[mean.front] = data[i]/mean.size;

// increment the front of the queue
		mean.front = (mean.front + 1 + mean.size )%mean.size;
	}
	return mean;
}
