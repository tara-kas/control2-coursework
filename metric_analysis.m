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
csv_file = 'experiment_csv/Baseline.csv';           % Input CSV file name
settling_threshold = 2.0;        % Settling band for alpha (±degrees from 0)
saturation_limit = 10.0;         % Voltage saturation limit (V)
theta_setpoint = 0;              % Arm angle setpoint (degrees)

%% Load Data
fprintf('Loading data from: %s\n', csv_file);
data = readtable(csv_file);

time = data.Time;
theta = data.Theta_2;            % Arm angle
alpha = data.Alpha;              % Pendulum angle
input_v = data.Input;            % Control voltage

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

%% 4. Maximum Arm Angle (Overshoot Percentage)
% Calculate overshoot relative to setpoint
% Overshoot % = ((Max - Setpoint) / Setpoint) * 100
% Since setpoint is 0, we report max angle and percentage of typical max swing

max_theta = max(abs(theta));
min_theta = min(theta);
max_theta_positive = max(theta);

% For overshoot percentage, we use the maximum arm angle achieved
% Referenced against a nominal expected maximum (from baseline ~23.2 deg)
nominal_max_theta = 23.2;  % Baseline reference (degrees)
overshoot_pct = ((max_theta_positive - nominal_max_theta) / nominal_max_theta) * 100;

fprintf('Max Arm Angle (θ): %.1f deg\n', max_theta_positive);
fprintf('Arm Angle Overshoot: %.1f %% (relative to %.1f° baseline)\n', overshoot_pct, nominal_max_theta);

%% 5. Steady State Error
% Calculate steady state error for alpha (pendulum angle)
% Use last 10% of data to determine steady state value
steady_state_samples = round(0.1 * length(alpha));
alpha_ss = mean(alpha(end-steady_state_samples:end));
alpha_setpoint = 0;  % Target pendulum angle
steady_state_error = abs(alpha_ss - alpha_setpoint);
fprintf('Steady State Error (α): %.3f deg\n', steady_state_error);

%% Summary Table
fprintf('\n--- SUMMARY TABLE ---\n');
fprintf('%-25s | %s\n', 'Metric', 'Value');
fprintf('%s\n', repmat('-', 1, 45));
fprintf('%-25s | %.2f s\n', 'Settling Time (α→0°)', settling_time);
fprintf('%-25s | %.2f V\n', 'RMS Input', rms_input);
fprintf('%-25s | %.1f %%\n', 'Saturation (±10V)', saturation_pct);
fprintf('%-25s | %.1f deg\n', 'Max Arm Angle (θ)', max_theta_positive);
fprintf('%-25s | %.1f %%\n', 'Overshoot', overshoot_pct);
fprintf('%-25s | %.3f %%\n', 'Steady State Error', steady_state_error);

%% Plotting
figure('Position', [100, 100, 1200, 800]);

% Plot 1: Arm Angle
subplot(2,2,1);
plot(time, theta, 'b', 'LineWidth', 1.2);
hold on;
yline(0, 'k--', 'LineWidth', 1);
if ~isnan(settling_idx)
    xline(settling_time, 'r--', 'LineWidth', 1.5);
end
xlabel('Time (s)');
ylabel('\theta (deg)');
title('Arm Angle');
grid on;
legend('θ', 'Setpoint', 'Settling Time', 'Location', 'best');

% Plot 2: Pendulum Angle
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
title('Pendulum Angle');
grid on;
legend('α', 'Setpoint', 'Settling Band', '', 'Settling Time', 'Location', 'best');

% Plot 3: Control Voltage
subplot(2,2,3);
plot(time, input_v, 'b', 'LineWidth', 1.2);
hold on;
yline(saturation_limit, 'r--', 'LineWidth', 1.5);
yline(-saturation_limit, 'r--', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('Input (V)');
title(sprintf('Control Voltage (RMS: %.2f V, Sat: %.1f%%)', rms_input, saturation_pct));
grid on;
legend('Input', 'Saturation Limits', '', 'Location', 'best');

% Plot 4: Zoomed Pendulum Angle (around settling)
subplot(2,2,4);
if ~isnan(settling_time)
    zoom_start = max(0, settling_time - 2);
    zoom_end = min(time(end), settling_time + 4);
else
    zoom_start = 5;
    zoom_end = 10;
end
zoom_mask = (time >= zoom_start) & (time <= zoom_end);
plot(time(zoom_mask), alpha(zoom_mask), 'b', 'LineWidth', 1.2);
hold on;
yline(0, 'k--', 'LineWidth', 1);
yline(settling_threshold, 'g--', 'LineWidth', 1);
yline(-settling_threshold, 'g--', 'LineWidth', 1);
if ~isnan(settling_idx)
    xline(settling_time, 'r--', 'LineWidth', 1.5);
end
xlabel('Time (s)');
ylabel('\alpha (deg)');
title('Pendulum Angle (Zoomed)');
grid on;

sgtitle(sprintf('MPC Analysis: %s', csv_file), 'FontSize', 14, 'FontWeight', 'bold');

% %% Export Results to Struct
% results.file = csv_file;
% results.settling_time = settling_time;
% results.rms_input = rms_input;
% results.saturation_pct = saturation_pct;
% results.max_theta = max_theta_positive;
% results.overshoot_pct = overshoot_pct;
% 
% fprintf('\nResults stored in "results" struct.\n');