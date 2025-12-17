function uout = mpc_controller(inputs)

curr_x = inputs(1:4);
curr_r = 0;
t = inputs(5);

persistent Controller
persistent last_u

if t == 0

    %% Plant model
    % Discrete time state space model with sampling time ts
    ts = 0.002;
    
    % A, B matrices found from state space calculations
    A = [1 0 0.0020 0;
         0 1 0 0.0020;
         0 0.1103 0.9909 -0.0004;
         0 0.3372 -0.0090 0.9989];

    B = [0 0 0.0412 0.0407]';

    % Output matrix selecting position states
    C = [1 0 0 0;
         0 1 0 0];

    % No direct feedthrough
    D = 0;

    %% Steady state mapping
    % Computes steady state xss and uss satisfying dynamics and output reference
    Mss = [(A - eye(4)) B;
           C zeros(2,1)];

    %% Cost weights and horizon
    % Q penalises state tracking error R penalises input deviation
    % Baseline values:
    Q = [4 0 0 0;
         0 3 0 0;
         0 0 1 0;
         0 0 0 1];      % penalises state deviation or tracking error

    R = 5;              % penalises control effort

    N = 10;             % prediction horizon
    
    % Input rate limit
    du_max = 5;

    %% Constraints
    % Hard bounds on input and state
    % Input magnitude limits / saturation
    u_min = -10;
    u_max = 10;

    x_min = [-2*pi; -2*pi; -inf; -inf];
    x_max = -x_min;

    %% Terminal cost
    % LQR based terminal weight for stability
    [K, P, ev] = dlqr(A, B, Q, R);

    %% YALMIP variables
    % Decision variables states and inputs over horizon
    yalmip('clear')

    u = sdpvar(repmat(1,1,N), repmat(1,1,N));
    x = sdpvar(repmat(4,1,N+1), repmat(1,1,N+1));

    % Parameters initial state reference and previous input
    x_init = sdpvar(4,1);
    r = sdpvar(1,1);
    u_prev = sdpvar(1,1);

    % Steady state targets as affine functions of reference
    uxss = Mss \ [0; 0; 0; 0; 0; r];
    xss = uxss(1:4);
    uss = uxss(5);

    constraints = [];
    objective = 0;

    % Initial condition constraint
    constraints = [constraints, x{1} == x_init];

    %% Soft state constraints option
    use_soft_state = true;
    rho_soft = 1000;

    if use_soft_state
        e = sdpvar(repmat(4,1,N), repmat(1,1,N));
    end

    %% Dynamics cost and constraints
    for k = 1:N

        % Stage cost
        objective = objective + (xss - x{k})' * Q * (xss - x{k}) ...
                               + (uss - u{k})' * R * (uss - u{k});

        % System dynamics
        constraints = [constraints, x{k+1} == A*x{k} + B*u{k}];

        % Input saturation
        constraints = [constraints, u_min <= u{k} <= u_max];

        % Input rate constraint
        if k == 1
            constraints = [constraints, -du_max <= u{k} - u_prev <= du_max];
        else
            constraints = [constraints, -du_max <= u{k} - u{k-1} <= du_max];
        end

        if ~use_soft_state
            % Hard state bounds
            constraints = [constraints, x_min <= x{k} <= x_max];
        else
            % Softened state bounds with slack penalty
            constraints = [constraints, x_min - e{k} <= x{k} <= x_max + e{k}, e{k} >= 0];
            objective = objective + rho_soft * (e{k}'*e{k});
        end
    end

    %% Terminal cost
    % Penalises terminal state deviation from steady state
    objective = objective + (xss - x{N+1})' * P * (xss - x{N+1});

    % Solver configuration
    ops = sdpsettings('solver','quadprog','verbose',2);

    % Parametric MPC controller
    Controller = optimizer(constraints, objective, ops, {x_init, r, u_prev}, u{1});

    if isempty(last_u)
        last_u = 0;
    end

    uout = Controller(curr_x, curr_r, last_u);
    last_u = uout;

else
    % Reuse compiled optimizer
    uout = Controller(curr_x, curr_r, last_u);
    last_u = uout;
end
