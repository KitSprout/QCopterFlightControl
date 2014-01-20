
%{
% mcc -m SensorDataPrint.m
% .EXE USE
function SensorDataPrint()

% clear all;
% clc;
% clf;

file = fopen('BLACKBOX.DAT', 'r');
ReadData = fread(file, 'int16');

fclose(file);

x = 1:1:size(ReadData);
plot(x,ReadData)

end
%}

%{
% SensorDataPrint('C:\Users\Hom\Desktop\BlackBox\BLACKBOX.DAT')
% SensorDataPrint('E:\My Research\My Programs\MATLAB\BlackBox\BLACKBOX.DAT')
function SensorDataPrint( path )

clear all;
clc;
clf;

file = fopen(path, 'r');
ReadData = fread(file, 'int16');

fclose(file);

x = 1:1:size(ReadData);
plot(x,ReadData)

end
%}

function SensorDataPrint()

clear all;
clc;
clf;

file = fopen('E:\My Research\My Programs\MATLAB\BlackBox\BLACKBOX.DAT', 'r');
ReadData = fread(file, 'int16');

fclose(file);

x = 1:1:size(ReadData);

% Acc.X
plot(x, ReadData, 'R');
%{
hold on;

% Acc.Y
plot(x, ReadData*0.1 - 10000, 'G');

hold on;

% Acc.Z
plot(x, ReadData*0.5 + 10000, 'B')
%}
hold off;

legend('Acc.X', 'Acc.Y', 'Acc.Z');

grid on

end
