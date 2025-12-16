function u = PID_controller(alpha)
% theta -> base angle (rad)
% alpha -> pendulum angle (rad)
% thetadot -> base angular velocity (rad)
% alphadot -> pendulum angular velocity (rad)
% u -> output voltage

    Kp = 40;
    Ki = 0;
    Kd = 0.21;
    ts = 0.001;
    
    target = 0;
    
    persistent integral_sum prev_err
    if isempty(integral_sum)
        integral_sum = 0;
        prev_err = 0;
    end
    
    err = target - alpha;
    
    % P
    Ep = Kp * err;
    
    % I
    integral_sum = integral_sum + err * ts;
    Ei = Ki * integral_sum;
    
    % D
    derivative = (err - prev_err) / ts;
    Ed = Kd * derivative;
    
    u = Ep + Ei + Ed;
    
    prev_err = err;
end