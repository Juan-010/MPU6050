% File          : MatLab_Processing.m
% Programmers   : Juan Ignacio Sanchez Serantes
%                 Enrique Walter Philippeaux
% Created on	: 10/10/2024
% Description 	: Gyroscope and accelerometer data processing for neat visualization
close all;
clear;
clc;
data = readtable('data2.csv');

%%
counter = data.Counter;
% Conversión para X
X_accel = convert_data(data.XAccel);
Y_accel = convert_data(data.YAccel);
Z_accel = convert_data(data.ZAccel);

X_gyro = convert_data(data.XGyro);
Y_gyro = convert_data(data.YGyro);
Z_gyro = convert_data(data.ZGyro);



ACCEL_SCALE = 16384.0/9.8;
GYRO_SCALE = 131;
fs = 10;

X_accel = X_accel ./ ACCEL_SCALE;
Y_accel = Y_accel ./ ACCEL_SCALE;
Z_accel = Z_accel ./ ACCEL_SCALE;
X_gyro = X_gyro ./ GYRO_SCALE;
Y_gyro = Y_gyro ./ GYRO_SCALE;
Z_gyro = Z_gyro ./ GYRO_SCALE;
t = (0:1/fs:(length(X_accel)-1)/fs);

X_accel = compensate(X_accel, 10);
Y_accel = compensate(Y_accel, 10);
Z_accel = compensate(Z_accel, 10);

X_gyro = compensate(X_gyro, 10);
Y_gyro = compensate(Y_gyro, 10);
Z_gyro = compensate(Z_gyro, 10);
%%
figure;
hold on;
plot(t, X_accel, 'r');  % Gráfico en rojo para X
plot(t, Y_accel, 'g');  % Gráfico en verde para Y
plot(t, Z_accel, 'b');  % Gráfico en azul para Z
hold off; 
xlabel('Tiempo (s)');
ylabel('Aceleración (m/s^2)');
title('Aceleración en los ejes X, Y y Z');
legend('Aceleración en X', 'Aceleración en Y', 'Aceleración en Z');
grid on;
xlim([t(1); t(end)]);
%%
figure;
hold on; 
plot(t, X_gyro, 'r');  % Gráfico en rojo para X
plot(t, Y_gyro, 'g');  % Gráfico en verde para Y
plot(t, Z_gyro, 'b');  % Gráfico en azul para Z
hold off; 
xlabel('Tiempo (s)');
ylabel('Velocidad angular (deg/s)');
title('Velocidad angular en los ejes X, Y y Z');
legend('\omega en X', '\omega en Y', '\omega en Z');
grid on;
xlim([t(1); t(end)]);
%%
figure;
hold on;
plot(t, counter, 'r');
hold off; 
xlabel('Tiempo (s)');
ylabel('Contador');
title('Contador');
grid on;
xlim([t(1); t(end)]);

%%
%Obtencion de rotaciones
Xg_int = cumsum(X_gyro* 1/fs); 
Yg_int = cumsum(Y_gyro* 1/fs);
Zg_int = cumsum(Z_gyro* 1/fs);


figure;
hold on;
plot(t, Xg_int, 'r');  % Gráfico en rojo para X
plot(t, Yg_int, 'g');  % Gráfico en verde para Y
plot(t, Zg_int, 'b');  % Gráfico en azul para Z
hold off; 
xlabel('Tiempo');
ylabel('Rotacion');
title('Angulo de rotacion en los ejes X, Y y Z');
legend('\Theta en X', '\Theta en Y', '\Theta en Z');
xlim([t(1); t(end)]);
grid on;





