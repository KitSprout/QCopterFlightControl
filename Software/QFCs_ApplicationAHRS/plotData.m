clc;
clear;
close all;

% load data
load('rawData_20170416_144700.mat');
gyr  = data( 1:  3, :);
acc  = data( 4:  6, :);
mag  = data( 7:  9, :);
q    = data(10: 13, :) / 10000;
sec  = data(14, :);
msc  = data(15, :) / 1000;
time = sec + msc;
time = time - time(1);

dataInfo = [ sprintf('lens = %d', dataLens), sprintf('t = %.2fs', time(end)), dataIndex ]

%{
% check time
dt = fix((time(2:end) - time(1:end-1)) * 1e3 + 1e-5) / 1e3;
% plot(1:dataLens-1, dt);
index = find(dt ~= mode(dt));
errTime = [index, time(index), time(index + 1)]
%}

% {
fig1 = figure(1);

subplot(4, 1, 1);
hold on
grid on
plot(time, gyr);
legend('gyr_x', 'gyr_y', 'gyr_z');
xlabel('time (s)');
ylabel('Angular velocity (dps)');

subplot(4, 1, 2);
hold on
grid on
plot(time, acc);
legend('acc_x', 'acc_y', 'acc_z');
xlabel('time (s)');
ylabel('Acceleration (m/s^2)');

subplot(4, 1, 3);
hold on
grid on
plot(time, mag);
legend('mag_x', 'mag_y', 'mag_z');
xlabel('time (s)');
ylabel('Magnetic (gauss)');

subplot(4, 1, 4);
hold on
grid on
plot(time, q);
legend('q0', 'q1', 'q2', 'q3');
xlabel('time (s)');
ylabel('quaternion');
%}
