#ifndef MEAN_H
#define MEAN_H

struct data init_running_mean(int M, int blocksize);
struct data calc_running_mean(struct data mean, float data[]);
struct data{
	float *mean, *data; 
	int blocksize, size, front;
};
#endif
