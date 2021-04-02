% Task 2
% Tyler Aubin, Aidan Greenlee, Caleb Jewell, Sam King

% Set N value (Arbitrary, Higher n = more accurate filter)
N = 1001;
w = 2 * pi * (0:N-1)/N;

% z = e^jomega
z = exp(1i * w);

% Number of poles = number of zeros
% filter f below 0.1 and above 0.35

poles = [
    0.9 * exp(1i * 2 * pi * 0.15 * [-1, 1]),...
    0.88 * exp(1i* 2 * pi * 0.25 * [-1,1]),...
    0.8 * exp(1i*2*pi*0.23 * [-1,1]),...
    0.89 * exp(1i*2*pi*0.145 * [-1,1])        
];

zeros = [
        0.99 * exp(1i* 2 * pi * 0.092 * [-1,1]),...
        0.88 * exp(1i* 2 * pi * 0.35 * [-1,1]),...
        0.85 * exp(1i* 2 * pi * 0.4 * [-1,1]),...
        0.9 * exp(1i* 2 * pi * 0.05 * [-1,1])
        
];



zeros = zeros';
poles = poles';
% Plot the pole-zero diagram
figure(1);
hold on;
plot(z)
plot(real(poles), imag(poles), 'x')
plot(real(zeros), imag(zeros), 'o')
title('Zero pole diagram')

% Help make it a bit more readable
axis square;
axis([-2 2 -2 2])
xlabel('Re(z)')
ylabel('Im(z)')

hold off;

% Find the gain of the filter.

% First get numerator and denominator in polynomial form (for
% simplification)
den = poly(poles);
num = poly(zeros);

f = linspace(0, 0.5, N);
w = 2 * pi * f;
z = exp(1i * w);

% H_z = |num / den| for gain
H_z = abs(polyval(num, z) ./ polyval(den, z));


H_z_DC = (1 / max(H_z)) .* H_z;

% With the coefficients from num and den
% plot in dB

H_z_DB = 20 * log10(H_z_DC);

figure(2);
hold on;

% Constrained frequencies
f_low = 0.1;
f_high = 0.35;

% Cutoffs
f_c1 = 0.15;
f_c2 = 0.25;

% Now patch everything (These are the 3 shapes seen in the figure from lab
% handout)
patch([0 f_low f_low 0], [-60, -60, -40, -40], 0.8 * [1, 1, 1]);
patch([f_high .5 .5 f_high], [-60, -60, -50, -50], 0.8*[1, 1, 1]);
patch([f_c1 .2 f_c2 f_c2 .2 f_c1], [-1, -6, -1, 1, -4, 1], 0.8 * [1, 1, 1]);
% And plot the filter
% disp(H_z_DB);
plot(f, H_z_DB);
% hold on;
title('Gain vs Frequency');
xlabel('Frequency (f)')
ylabel('Gain (dB)')
hold off;