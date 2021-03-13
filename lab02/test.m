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

for i=1:1000
    x(i) = .2*sin(i*pi/100);
end
output = sosfilt(bq_coef, x);
plot(x,'r','LineWidth',2)
hold on
g = 1/max(output)
plot(g*output,'y','LineWidth',2)
hold on
output = conv(fir_coef,x);
plot(output,'Color','#00AAE3','LineWidth',2)
grid on
lgnd = legend('input','biquad','fir');
set(lgnd, 'Color', 'w');
set(gca, 'GridColor','w');
set(gca, 'Color', 'k');
xlim([0,length(fir_out)]);