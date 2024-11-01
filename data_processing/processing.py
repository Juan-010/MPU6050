# File          : processing.py
# Programmers   : Juan Ignacio Sanchez Serantes
#                 Enrique Walter Philippeaux
# Created on	: 10/10/2024
# Description 	: Script to process the data from the capture.txt file and save it into a CSV file
import os
import sys
import csv

#open the file
file = open("capture.txt", "r")

#remove all \n
data = file.read().replace("\n", "")

#place data into a vector with 28 characters each
data = [data[i:i+28] for i in range(0, len(data), 28)]

#divide into clumps, 1B, counter, 6 data points
data = [[data[i][0:2], data[i][2:4], data[i][4:8], data[i][8:12], data[i][12:16], data[i][16:20], data[i][20:24], data[i][24:28]] for i in range(len(data))]

#convert to int16
data = [[int(data[i][j], 16) for j in range(8)] for i in range(len(data))]

#Create and fill CSV
with open('data2.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["1B", "Counter", "X Accel", "Y Accel", "Z Accel", "X Gyro", "Y Gyro", "Z Gyro"])
    writer.writerows(data)
