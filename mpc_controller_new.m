function uout = mpc_controller_new(inputs)
% MPC with steady‑state target tracking, LQR terminal cost, and constraints.
%
% LEARNING GOALS (students):
%   1) Build an offset‑free MPC by tracking steady‑state targets (xss, uss).
%   2) Add hard constraints (box bounds) and understand feasibility issues.
%   3) Add soft constraints with slack variables to keep feasibility, and weight them.
%   4) Use an LQR terminal cost P to stabilise short horizons N.
%
% Inputs:
%   currentx  - current state [theta; theta_dot]
%   currentr  - reference alpha (scalar)
%   t         - time step (t == 0 initialises controller)
% Output:
%   uout      - optimal control input

curr_x = inputs(1:4);
curr_r = 0;
t = inputs(5);

persistent Controller  % Persistent optimizer object for re-use

if t == 0
    % --- System Initialization (executed only once) ---
    %% --- 1) Plant model (discrete-time) ------------------------------ 
    % TODO [Students]: Fill in actual system identification values
    % These parameters should come from the discrete-time model of your system
    % Example: For a DC motor position system, a and b are from identified A,B matrices.
    % ts is the sampling time in seconds.
    ts = 0.002;      % Sampling time [s]  

    % Discrete-time state-space model
    % A, B, C, D discrete matrices
    A = [1,   0,   0.0020,   0;
         0,   1.0000,   0,   0.0020;
         0, 0.1103, 0.9909, -0.0004;
         0, 0.3372, -0.0090, 0.9989];

    B = [0, 0, 0.0412, 0.0407]';

    % CHECK THIS LOGIC v
    C = [1, 0, 0, 0;      % since C in form [I 0],
         0, 1, 0, 0];     % want both theta and alpha in output vector

    D = 0;                % no direct feedthrough


    %% --- 2) Steady-state mapping M for (xss, uss) given r -----------
    % Solve:
    %   (A - I)*xss + B*uss = 0
    %   C*xss = r
    % Pack into M * [xss; uss] = [0; 0; r]

    % Mss dimensions should be 6x5 (A is 4x4, C is 2x4 => 4+2 = 6)
                                %  ([xss; uss] is 4x1+1x1 = 5)
    Mss = [(A-eye(4)) B;
           C          zeros(2,1)];
    
    %% --- 3) MPC cost weights & horizon -------------------------------
    % Q weights the state tracking error (x - xss).
    % R weights the input tracking error (u - uss).

    % since (x-xss) is 4x1, Q should be 4x4
    % Q = eye(4); %REPLACE W/ GAINS
    Q = [4 0 0 0;
         0 3 0 0;
         0 0 1 0;
         0 0 0 1];
        
    % R weights the input tracking err (u - uss)
    % since (u-uss) is 1x1, R should be scalar
    R = 5;      %REPLACE W/ GAINS

    % prediction horizon
    N = 10;      %REPLACE W/ GAINS
    
    
    %% --- 4) Constraints (hard limits) --------------------------------
    % Input bounds (actuator saturation)

    u_min = -10;   % e.g., -10 V (or Nm)
    u_max =  10;

    % State (position/velocity) box bounds:
    x_min = [-2*pi; -2*pi; -inf; -inf];   %REPLACE
    x_max = -x_min;       %REPLACE

                      
    %% --- 5) Terminal cost from LQR (stability) -----------------------
    % P solves the discrete Riccati equation for (A,B,Q,R).
    % Adding (x_N+1 - xss)' P (x_N+1 - xss) helps stability with short N.

    % TODO [Students]: Replace eye(2) with result from dlqr().

    [K, P, ev] = dlqr(A, B, Q, R);  % <-- Temporary placeholder

    %% --- 6) Build the YALMIP problem --------------------------------
    yalmip('clear')  % avoid variable accumulation across rebuilds

    % Decision variables over the horizon
    u = sdpvar(repmat(1,1,N),   repmat(1,1,N));    % u{1}..u{N}
    x = sdpvar(repmat(4,1,N+1), repmat(1,1,N+1));  % x{1}..x{N+1}
    sdpvar r                                        % reference (parameter)



    % Steady-state target as affine function of r (use "\" not inv())
    uxss = Mss \ [0; 0; 0; 0; 0; r];
    xss  = uxss(1:4);
    uss  = uxss(5);


    % Build cost and constraints
    constraints = [];
    objective = 0;

    % initial condition: x{1} is the current state
    constraints = [constraints, x{1} == curr_x];

    %% --- 7) Optional soft state constraints --------------------------
    % If hard state bounds make the QP infeasible, introduce slack e{k} >= 0
    % to "soften" the box:  x_min - e{k} <= x{k} <= x_max + e{k}
    % and penalise ||e{k}||^2 with a large weight rho_soft.

    use_soft_state = true; % set false to enforce hard state bounds
    rho_soft = 1000; % larger -> closer to hard bounds
    if use_soft_state
        e = sdpvar(repmat(4,1,N), repmat(1,1,N)); % same dimension as x{k}
    end

    %% --- 8) Stage cost, dynamics, and constraints --------------------
    for k = 1:N
        % Stage cost: track xss and uss
        objective = objective + (xss - x{k})' * Q * (xss - x{k}) ...
                               + (uss - u{k})' * R * (uss - u{k});
        % System dynamics
       constraints = [constraints, x{k+1} == A*x{k} + B*u{k}];


        % Input bounds (hard)
       %  u_min <= u{k} <= u_max
       % TODO [Students]: Replace with u_min <= u{k} <= u_max.
        constraints = [constraints, u_min <= u{k} <= u_max]; 

       if ~use_soft_state

       % Hard state box (can cause infeasibility if too tight)
       % TODO [Students]: Replace [] with x_min <= x{k} <= x_max.
          constraints = [constraints, x_min <= x{k} <= x_max];
       
       else
            % Soft state box with slack; keep e{k} >= 0； x_min - e{k} <= x{k} <= x_max + e{k}
             % TODO [Students]: Replace [] with x_min - e{k} <= x{k} <= x_max + e{k}, e{k} >= 0.
            constraints = [constraints, x_min - e{k} <= x{k} <= x_max + e{k}, e{k} >= 0];
            % Penalise slack in the cost

             % TODO [Students]: Replace 0 with rho_soft * (e{k}'*e{k})
            objective   = objective + rho_soft * (e{k}'*e{k});
        end
        

    end
 

   %% --- 9) Terminal cost on terminal state error --------------------
    % TODO [Students]: Add terminal cost term using P
    % This term should penalise (xss - x{N+1}) using P.
    objective = objective + (xss - x{N+1})'*P*(xss - x{N+1});  % <-- Replace 0 with terminal cost term

    % Solver settings
    ops = sdpsettings('solver','quadprog','verbose',2);

    % Create the parametric optimizer: inputs are initial state and reference
    Controller = optimizer(constraints, objective, ops, r, u{1});

    % First solve
    uout = Controller(curr_r);
    % if problem
    %     warning('MPC solver failed during initialization (code %d).', problem);
    %     uout = 0;
    % end

else
    % Subsequent calls reuse the compiled optimizer for speed
    uout = Controller(curr_r);
    % if problem
    %     warning('MPC solver failed at runtime (code %d).', problem);
    %     uout = 0;
    % end
end
