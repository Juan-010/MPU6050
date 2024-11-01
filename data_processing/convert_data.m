% File          : convert_data.m
% Programmers   : Juan Ignacio Sanchez Serantes
%                 Enrique Walter Philippeaux
% Created on	: 10/10/2024
% Description 	: MatLab function to convert raw data into double for processing

function [data] = convert_data(raw_data)
data = uint16(raw_data);    % Specifies uint16 for data processing
data = swapbytes(data);     % Fixes endianness
data = typecast(data, 'int16'); % Converts to signed int. Same binary data
data = double(data); % Converts natural number into double for real value computing