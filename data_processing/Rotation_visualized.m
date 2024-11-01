% File          : Rotation_visualized.m
% Programmers   : Juan Ignacio Sanchez Serantes
%                 Enrique Walter Philippeaux
% Created on	: 10/10/2024
% Description 	: Gyroscope data used to visualize rotation in 3D space

close all;
clc;

% Parameters
theta_x = Xg_int.*pi/180;  % Rotation around X axis per frame (in radians)
theta_y = Yg_int.* pi/180;  % Rotation around Y axis per frame (in radians)
theta_z = Zg_int.* pi/180;  % Rotation around Z axis per frame (in radians)

% Define the vertices of a unit cube
vertices = [-1 -1 -1;
             1 -1 -1;
             1  1 -1;
            -1  1 -1;
            -1 -1  1;
             1 -1  1;
             1  1  1;
            -1  1  1];

% Define the faces of the cube
faces = [1 2 3 4;
         5 6 7 8;
         1 2 6 5;
         2 3 7 6;
         3 4 8 7;
         4 1 5 8];

% Set up the figure
figure;
h = patch('Vertices', vertices, 'Faces', faces, ...
          'FaceColor', 'cyan', 'EdgeColor', 'black');
axis([-2 2 -2 2 -2 2]);
axis vis3d;
grid on;

% Rotation matrices for x, y, z axes
Ry = @(theta) [1 0 0;
               0 cos(theta) -sin(theta);
               0 sin(theta) cos(theta)];
           
Rz = @(theta) [cos(theta) 0 sin(theta);
               0 1 0;
               -sin(theta) 0 cos(theta)];
           
Rx = @(theta) [cos(theta) -sin(theta) 0;
               sin(theta) cos(theta) 0;
               0 0 1];

% Animation loop

for t = 1:length(theta_x)
    % Compute the rotation matrix for this frame
    R = Rx(theta_x(t)) * Ry(theta_y(t)) * Rz(theta_z(t));
    
    % Rotate the vertices
    rotated_vertices = (R * vertices')';
    
    % Update the cube vertices
    set(h, 'Vertices', rotated_vertices);
    
    % Pause for a short duration to create animation effect
    pause(0.1);
end
