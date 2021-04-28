% Tyler Aubin, Caleb Jewell, Sam King, Aidan Greenlee

% Using size of fft array
M=1280;

% Arbitrary beta
beta = 2;

% Generate the filter
win = kaiser(M,beta)';

% Print out the coefficients
for i = 1:length(M(:,1))-1
    fprintf("%g, %g, %g, %g, %g, \n", win(i,:));
  
end
sum(win)

fprintf("%g, %g, %g, %g, %g, \n", win(end,:));