%% MPC Pendulum Control Data Analysis
% This script analyzes CSV data from MPC controller experiments
% Calculates: Settling Time, RMS Input, Saturation %, Max Arm Angle Overshoot
%
% CSV Format Expected: Time, Theta_1, Theta_2, Alpha, Input
%   - Time: timestamp in seconds
%   - Theta_2: Arm angle (degrees)
%   - Alpha: Pendulum angle (degrees), starts at -180, settles to 0
%   - Input: Control voltage (V), saturates at ±10V

clear; clc; close all;

%% User Configuration
% Modify these parameters as needed
csv_file = 'experiment_csv/input_mag_5.csv';           % Input CSV file name
settling_threshold = 2.0;        % Settling band for alpha (±degrees from 0)
saturation_limit = 10.0;         % Voltage saturation limit (V)
theta_setpoint = 0;              % Arm angle setpoint (degrees)
ts = 0.002;                      % Sampling time (s)

%% Load Data
fprintf('Loading data from: %s\n', csv_file);
data = readtable(csv_file);

time = data.Time;
theta = data.Theta_2;            % Arm angle
alpha = data.Alpha;              % Pendulum angle
input_v = data.Input;            % Control voltage

% Calculate input rate (delta u)
delta_u = [0; diff(input_v)] / ts;  % V/s

fprintf('Data loaded: %d samples, %.2f seconds duration\n', length(time), time(end));

%% 1. Settling Time (alpha -> 0°)
% Find when alpha enters and stays within ±threshold of 0 degrees
settled = abs(alpha) < settling_threshold;

settling_time = NaN;
settling_idx = NaN;

for i = 1:length(settled)
    if settled(i)
        % Check if it stays settled for remaining samples (95% criteria)
        remaining = settled(i:end);
        if mean(remaining) > 0.95
            settling_time = time(i);
            settling_idx = i;
            break;
        end
    end
end

fprintf('\n--- RESULTS ---\n');
if ~isnan(settling_time)
    fprintf('Settling Time (α → 0°): %.2f s\n', settling_time);
else
    fprintf('Settling Time: NOT ACHIEVED\n');
end

%% 2. RMS Input Voltage
rms_input = sqrt(mean(input_v.^2));
fprintf('RMS Input: %.2f V\n', rms_input);

%% 3. Saturation Percentage
% Time spent at ±10V limits
saturated = abs(input_v) >= (saturation_limit - 0.01);  % Small tolerance
saturation_pct = 100 * sum(saturated) / length(input_v);
fprintf('Saturation (±%.0fV): %.1f %%\n', saturation_limit, saturation_pct);

%% 4. Maximum Arm Angle
max_theta = max(theta);
fprintf('Max Arm Angle (θ): %.1f deg\n', max_theta);

%% 5. Steady State Error
% Calculate steady state error for alpha (pendulum angle)
% Use last 10% of data to determine steady state value
steady_state_samples = round(0.1 * length(alpha));
alpha_ss = mean(alpha(end-steady_state_samples:end));
alpha_setpoint = 0;  % Target pendulum angle
steady_state_error = abs(alpha_ss - alpha_setpoint);
fprintf('Steady State Error (α): %.3f deg\n', steady_state_error);

%% 6. Rise Time
% Rise time: time to go from 10% to 90% of the total change
% Alpha goes from -180 deg to 0 deg, so total change is 180 deg
alpha_initial = -180;
alpha_final = 0;
alpha_10pct = alpha_initial + 0.1 * (alpha_final - alpha_initial);  % -162 deg
alpha_90pct = alpha_initial + 0.9 * (alpha_final - alpha_initial);  % -18 deg

% Find time when alpha first crosses 10% threshold
idx_10 = find(alpha >= alpha_10pct, 1, 'first');
% Find time when alpha first crosses 90% threshold
idx_90 = find(alpha >= alpha_90pct, 1, 'first');

if ~isempty(idx_10) && ~isempty(idx_90)
    rise_time = time(idx_90) - time(idx_10);
    fprintf('Rise Time (10%%-90%%): %.3f s\n', rise_time);
else
    rise_time = NaN;
    fprintf('Rise Time: NOT ACHIEVED\n');
end

%% Summary Table
fprintf('\n--- SUMMARY TABLE ---\n');
fprintf('%-25s | %s\n', 'Metric', 'Value');
fprintf('%s\n', repmat('-', 1, 45));
fprintf('%-25s | %.3f s\n', 'Rise Time (10%-90%)', rise_time);
fprintf('%-25s | %.2f s\n', 'Settling Time (α→0°)', settling_time);
fprintf('%-25s | %.3f deg\n', 'Steady State Error', steady_state_error);
fprintf('%-25s | %.2f V\n', 'RMS Input', rms_input);
fprintf('%-25s | %.1f %%\n', 'Saturation (±10V)', saturation_pct);
fprintf('%-25s | %.1f deg\n', 'Max Arm Angle (θ)', max_theta);

%% Plotting - Clear and labelled time-domain plots
figure('Position', [100, 100, 1200, 800]);

% Common axis limits for consistent comparison
time_limits = [0 time(end)];

% Plot 1: Arm Angle θ(t)
subplot(2,2,1);
plot(time, theta, 'b', 'LineWidth', 1.2);
hold on;
yline(0, 'k--', 'LineWidth', 1);
xlabel('Time (s)');
ylabel('\theta (deg)');
title('Arm Angle \theta(t)');
xlim(time_limits);
grid on;
legend('\theta', 'Setpoint', 'Location', 'best');

% Plot 2: Pendulum Angle α(t)
subplot(2,2,2);
plot(time, alpha, 'b', 'LineWidth', 1.2);
hold on;
yline(0, 'k--', 'LineWidth', 1);
yline(settling_threshold, 'g--', 'LineWidth', 1);
yline(-settling_threshold, 'g--', 'LineWidth', 1);
if ~isnan(settling_idx)
    xline(settling_time, 'r--', 'LineWidth', 1.5);
end
xlabel('Time (s)');
ylabel('\alpha (deg)');
title('Pendulum Angle \alpha(t)');
xlim(time_limits);
grid on;
legend('\alpha', 'Setpoint', 'Settling Band', '', 'Settling Time', 'Location', 'best');

% Plot 3: Control Voltage u(t)
subplot(2,2,3);
plot(time, input_v, 'b', 'LineWidth', 1.2);
hold on;
yline(saturation_limit, 'r--', 'LineWidth', 1.5);
yline(-saturation_limit, 'r--', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('u (V)');
title('Control Voltage u(t)');
xlim(time_limits);
ylim([-12 12]);
grid on;
legend('u', 'Saturation Limits', '', 'Location', 'best');

% Plot 4: Input Rate Δu(t)
subplot(2,2,4);
plot(time, delta_u, 'b', 'LineWidth', 1.2);
hold on;
xlabel('Time (s)');
ylabel('\Delta u (V/s)');
title('Input Rate \Delta u(t)');
xlim(time_limits);
grid on;
legend('\Delta u', 'Location', 'best');

sgtitle(sprintf('MPC Analysis: %s', csv_file), 'FontSize', 14, 'FontWeight', 'bold');