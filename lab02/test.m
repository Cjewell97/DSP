bq_coef = [
		1, 1.618,1, 1, -1.5371, 0.9025,
		1, -0.618, 1, 1, 0.0, -0.81,
		1, 1.618,1, 1, -1.5371, 0.9025,
		1, -0.618, 1, 1, 0.0, -0.81,
		1, 1.618,1, 1, -1.5371, 0.9025,
		1, -0.618, 1, 1, 0.0, -0.81
	];

fir_coef = [
		1, .2, 1.2, -.3, .5,...
		1, .2, -1, 1.3, -.4,...
		.5, -.2, .5, 1.5, 1,...
		1, 1, -.7, 1.3, .9
	];

x = zeros(1000,1);
sine = false;
for i=1:1000
    if sine
        x(i) = .2*sin(i*pi/100)+.2;
    else
        if mod(floor(i/100),2);
            x(i) = .2;
        end
    end
end

bq_out = sosfilt(bq_coef, x);
plot(x,'r','LineWidth',2)
hold on
g = 1/max(bq_out)
plot(g*bq_out,'Color','#ffa600','LineWidth',2)
hold on
fir_out = conv(fir_coef,x);
plot(fir_out,'Color','#00AAE3','LineWidth',2)
xlim([0,length(fir_out)]);
grid on
lgnd = legend('input','biquad','fir');
xlabel('n')
ylabel('y[n]')
set(lgnd, 'Color', 'w');
% set(gca, 'GridColor','w');
% set(gca, 'Color', 'k');