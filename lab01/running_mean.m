mean = init_running_mean(8,3);
data = [0,1,2,3,4,5,6,7];
mean = calc_running_mean(mean, data);
disp(mean.mean);
mean = calc_running_mean(mean, data);
disp(mean.mean);

function mean = calc_running_mean(mean, data);
    for(i=1:mean.blocksize)
        mean.data(i) = data(i)/mean.size;
        mean.mean(i) = mean.mean(mod(i+mean.size - 1, mean.size) + 1) - mean.data(mod(mean.rear - 1+mean.size, mean.size) + 1) + mean.data(i);
        mean.rear = mod((mean.rear + 1), mean.size);
        %disp(mean.mean(i));
        result = mean.mean;
    end
end

function s = init_running_mean(M,blocksize)
    s.blocksize = blocksize;
    s.size = M;
    s.data = zeros(M, 1);
    s.mean = zeros(M, 1);
    s.rear = s.size - s.blocksize + 1;
end