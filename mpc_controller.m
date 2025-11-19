% MPC Controller design with YALMIP

% Brief: Implement a constrained MPC in YALMIP and evaluate it under varied design/constraint
% scenarios (e.g., with/without terminal cost; input magnitude and input-rate limits).

function y = mpc_controller(curr_x, curr_r, t, vm)
% INPUTS:
%   curr_x  - current state [theta; alpha; theta_dot; alpha_dot]
%   curr_r  - reference position (scalar)
%   t       - time step (t == 0 initialises controller)
%   vm      - control input: motor voltage
% OUTPUT:
%   y       - optimal control input [theta; alpha]

persistent Controller  % preserves optimiser for reuse and improve execution speed

if t == 0     
    %% initialise all the vars once
    % simulink
    ts = 0.002;         % sampling time (s)

    % motor
    Rm = 7.5;           % armature resistance (ohms)
    kt = 0.042;         % torque constant (Nm/A)
    km = 0.042;         % back-emf (Vs/rad)

    % rotary arm
    mr = 0.095;         % arm mass (kg)
    r = 0.085;          % arm length - pivot to pendulum joint (m)
    Jr = 2.41e-4;       % arm moment of inertia (kgm^2)
    br = 1e-3;          % arm viscous damping coeff (Nms/rad)

    % pendulum link
    mp = 0.024;         % pendulum mass (kg)
    Lp = 0.129;         % pendulum length (m)
    l = 0.0645;         % pendulum centre of mass (m)
    Jp = 1.33e-4;       % pendulum moment of inertia (kgm^2)
    bp = 5.0e-5;        % pendulum viscous damping coeff (Nms/rad)

    g = 9.81;           % gravitational constant (m/s^2)

    %% state variables and sub-vars
    M = [ (Jr + mp * r^2),  (-mp * l * r);
          (mp * l * r),     (-Jp) ];

    a31 = 0;
    a32 = (-mp^2 * l^2 * r * g) / det(M);
    a33 = (Jp * ((kt * km) / Rm + br)) / det(M);
    a34 = (bp * mp * l * r) / det(M);
    b31 = ((-kt * Jp) / Rm) / det(M);
    
    a41 = 0;
    a42 = (-(Jr + mp * r^2) * mp * g * l) / det(M);
    a43 = ((mp * l * r) * ((kt * km) / Rm) + br) / det(M);
    a44 = ((Jr + mp * r^2) * bp) / det(M);
    b41 = (-(mp * l * r) * (kt / Rm)) / det(M);

    % A, B, C, D matrices
    A = [0,   0,   1,   0;
         0,   0,   0,   1;
         a31, a32, a33, a34;
         a41, a42, a43, a44];

    B = [0, 0, b3, b4]';

    % CHECK THIS LOGIC v
    C = [1, 0, 0, 0;      % since C in form [I 0],
         0, 1, 0, 0];     % want both theta and alpha in output vector

    D = 0;                % no direct feedthrough


    %% steady state mapping SSM
    % solves (A - I)*xss + B*uss = 0
    % C*xss = ref
    % => M*[xss; uss] = [0;0;0;0;r1;r2]

    % Mss dimensions should be 6x5 (A is 4x4, C is 2x4 => 4+2 = 6)
                                %  ([xss; uss] is 4x1+1x1 = 5)
    Mss = [(A-eye(4)) B;
           C          zeros(2,1)];


    %% MPC costs weights and horizon
    % Q weights the state tracking err (x - xss)
    % since (x-xss) is 4x1, Q should be 4x4
    Q = eye(4); %REPLACE W/ GAINS

    % R weights the input tracking err (u - uss)
    % since (u-uss) is 1x1, R should be scalar
    R = 1;      %REPLACE W/ GAINS

    % prediction horizon
    N = 1;      %REPLACE W/ GAINS


    % constraints (hard limits)
    % input bounds/actuator saturation
    u_min = -10;    %REPLACE
    u_max = 10;     %REPLACE


    % state bounds [theta; alpha; theta_dot; alpha_dot]
    x_min = [-2*pi; -2*pi; -10; -10];   %REPLACE
    x_max = [2*pi; 2*pi; 10; 10];       %REPLACE

   
    %% terminal cost from LQR
    % P solves the discrete Riccati equation for (A,B,Q,R)
    [~,P,~] = dlqr(A,B,Q,R,N);


    %% YALMIP time
    yalmip("clear");

    % decision vars over the time horizon


end

end