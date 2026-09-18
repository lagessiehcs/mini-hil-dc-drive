%% DC Motor Parameters
% Mini HIL DC Motor Project

%% Electrical parameters
R  = 2.0;       % Armature resistance [Ohm]
L  = 0.005;     % Armature inductance [H]

%% Motor constants
Kt = 0.05;      % Torque constant [N*m/A]
Ke = 0.05;      % Back-EMF constant [V*s/rad]

%% Mechanical parameters
J  = 1e-4;      % Rotor inertia [kg*m^2]
b  = 1e-4;      % Viscous friction [N*m*s/rad]

%% Supply / voltage test
V_supply   = 6.0;   % Motor supply voltage [V]
V_step_time = 0.1;  % Voltage applied at [s]

%% Load test
T_load      = 0.01; % Load torque [N*m]
T_step_time = 0.5;  % Load applied at [s]

%% Discrete simulation
Ts_plant = 0.001;    % Plant simulation sample time [s]