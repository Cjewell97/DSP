% Tyler Aubin, Sam King, Caleb Jewell, Aidan Greenlee
% Determine an adequate filter to reject/stop aliasing

% Given the passband of 1kHz, and stopband of 2.2kHz
N = 16*1024;

% Sampling Frequency (32kHz)
Fs = 32e3;

beta = 9;

% Set f and M
f = (0:N-1)/N;
M = 75;

% Frequency points on Nyquist
F = [0 0.03125 0.06875 0.5] / 0.5;

% Desired amplitude corresponding to F
A =[1 1 0 0];

% Delta p
d1 = 1 - 10^(-0.2/20);

% Delta s
d2 = 10^(-60/20); 

% Weights
W = [1/d1 1/d2];

% Create a PM Equiripple filter of M - 1 order
hpm = firpm(M-1, F, A, W);
Hpm = fft(hpm, N);
figure(1); clf;
%Second block spec patch for f = 0.125 to 0.5
patch([.06875 .5 .5 .06875],[-100 -100 -60 -60], .9*[1 1 1]);
%Pass band spec
patch([0.0 0.03125 0.03125 0.0],[-0.2 -0.2 0.2 0.2],.9*[1 1 1]);
grid on;
hold on;
plot(f, 20 * log10(abs(Hpm)), 'r');
xlim([0 0.5]);

for i = 1:length(hpm(:,1))-1
    fprintf("%g, %g, %g, %g, %g, \n", hpm(i,:));
    
end
fprintf("%g, %g, %g, %g, %g, \n", hpm(end,:));