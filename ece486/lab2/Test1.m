% Coefficients
ab = [1 1.618 1 1 -1.5371 0.9025; 1 -0.618 1 1 0 -0.81];

% Input
x = [3 1.5 -1.8 0.2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];

% Use a second order Transposed Direct 2 form Biquad filter
output = sosfilt(ab, x);

disp(output)