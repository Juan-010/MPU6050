% File          : compensate.m
% Programmers   : Juan Ignacio Sanchez Serantes
%                 Enrique Walter Philippeaux
% Created on	: 10/10/2024
% Description 	: MatLab function to compensate offset of Gyro and Accel data

function [data] = compensate(data_old, count)
cumdata = 0;
for i = 1:count
    cumdata = cumdata + data_old(i);
end
cumdata = cumdata / count;
data = data_old - cumdata;