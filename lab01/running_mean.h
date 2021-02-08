#ifnde MEAN_H
#define MEAN_H

struct data init_running_mean();
int running_mean();
struct data calc_running_mean();
struct data{
	float *mean, *data; 
	int blocksize, size, rear;
};
struct data running_queue();
#endif
