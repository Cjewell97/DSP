% data for testing
mean = init_running_mean(3,8);
data = [0,1,2,3,4,5,6,7];

% loop through 11 running mean calculations and create a stem plot
[y,mean] = calc_running_mean(mean, data);
for(i=1:10)
    [z,mean] = calc_running_mean(mean, data);
    y=cat(1,y,z);
end
stem(y)


%% Functions

% Initialize the running mean/circular queue
% - hold blocksize, and average size
% - create an array of M zeros for holding data
% - create an array of blocksize zeros for output blocks
% - create index marker for circular queue
function s = init_running_mean(M,blocksize)
    s.blocksize = blocksize;
    s.size = M;
    s.data = zeros(M, 1);
    s.mean = zeros(blocksize, 1);
    s.front = 1;
end

% Calculate the running mean with a circular queue
function [y,mean] = calc_running_mean(mean, data)

    % for each value of the input block:
    for(i=1:mean.blocksize)
        % Calculate the mean by adding the previous value of the mean to
        % the current data divided by the average size M. Subtract last piece
        % of data no longer being used by the running average
        mean.mean(i) = mean.mean(mod(i + mean.blocksize - 2, mean.blocksize) + 1) + data(i)/mean.size - mean.data(mod(mean.front + mean.size - 1, mean.size) + 1);
        
        % Save the data being averaged
        mean.data(mean.front) = data(i)/mean.size;
        
        % increment the front of the circular queue
        mean.front = mod((mean.front + mean.size), mean.size) + 1;
        
        y = mean.mean;
        %disp(mean.mean(i));
    end
end