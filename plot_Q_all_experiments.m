% --- Plot ALL Experimental Results (Shifted Grid + Boxed Legend) ---
% Features:
% 1. Grid shifted DOWN by 10% to create header space.
% 2. Legend is VERTICAL, CENTERED, and has a BOX OUTLINE.
% 3. No collisions.

clear; clc; close all;

% --- 1. Load Data ---
files = {
    'Baseline.csv', 'HighQ.csv', 'LowQ.csv', ...
    'HighTheta.csv', 'HighAlpha.csv', ...
    'HighArmVel.csv', 'HighPendVel.csv'
};

data = containers.Map();

for i = 1:length(files)
    if isfile(files{i})
        T = readtable(files{i});
        ts = 0.002;
        T.Delta_Input = [0; diff(T.Input) ./ ts]; 
        [~, name, ~] = fileparts(files{i});
        data(name) = T;
    end
end

if ~isKey(data, 'Baseline')
    error('Baseline.csv is missing!');
end

% --- 2. Generate Figures ---

% Figure 1: Global Q/R
create_final_figure('Global_QR_Scaling', ...
    data('Baseline'), 'Baseline', 'k', ...
    data('HighQ'),    'High Q (Aggressive)', 'r', ...
    data('LowQ'),     'Low Q (Conservative)', 'b');

fprintf('Saved "Global_QR_Scaling.png"\n');

% Figure 2: Position Weights
create_final_figure('Position_Weights_Effect', ...
    data('Baseline'),  'Baseline', 'k', ...
    data('HighTheta'), 'High Theta Penalty', [0.85, 0.33, 0.1], ... 
    data('HighAlpha'), 'High Alpha Penalty', 'b');

fprintf('Saved "Position_Weights_Effect.png"\n');

% Figure 3: Velocity Weights
create_final_figure('Velocity_Weights_Effect', ...
    data('Baseline'),    'Baseline', 'k', ...
    data('HighArmVel'),  'High Arm Damping', 'm', ...
    data('HighPendVel'), 'High Pend Damping', [0, 0.6, 0]); 

fprintf('Saved "Velocity_Weights_Effect.png"\n');
fprintf('Done!\n');


% --- Helper Function ---
function create_final_figure(FigName, T1, Name1, Col1, T2, Name2, Col2, T3, Name3, Col3)
    
    % Create Figure
    fig = figure('Name', FigName, 'Color', 'w', 'Position', [100, 50, 1000, 950]);
    
    lw = 1.0;
    lw_b = 3.0;
    shift_down = 0.08; % Amount to push the grid down (0.0 to 1.0 scale)
    
    % --- Subplot 1 (Top Left) ---
    ax1 = subplot(2,2,1);
    h1 = plot(T1.Time, T1.Theta_2, 'Color', Col1, 'LineWidth', lw_b); hold on;
    h2 = plot(T2.Time, T2.Theta_2, 'Color', Col2, 'LineWidth', lw, 'LineStyle', '--');
    h3 = plot(T3.Time, T3.Theta_2, 'Color', Col3, 'LineWidth', lw, 'LineStyle',':');
    ylabel('\theta (deg)'); xlabel('Time (s)');
    title([strrep(FigName, '_', ' ') ' - Arm Angle']);
    grid on;
    % Shift Down
    pos = get(ax1, 'Position'); 
    set(ax1, 'Position', [pos(1), pos(2) - shift_down, pos(3), pos(4)]);
    
    % --- Subplot 2 (Top Right) ---
    ax2 = subplot(2,2,2);
    plot(T1.Time, T1.Alpha, 'Color', Col1, 'LineWidth', lw_b); hold on;
    plot(T2.Time, T2.Alpha, 'Color', Col2, 'LineWidth', lw, 'LineStyle', '--');
    plot(T3.Time, T3.Alpha, 'Color', Col3, 'LineWidth', lw, 'LineStyle', ':');
    ylabel('\alpha (deg)'); xlabel('Time (s)');
    title([strrep(FigName, '_', ' ') ' - Pendulum Angle']);
    yline(0, 'k--', 'Target', 'HandleVisibility', 'off'); 
    grid on;
    % Shift Down
    pos = get(ax2, 'Position'); 
    set(ax2, 'Position', [pos(1), pos(2) - shift_down, pos(3), pos(4)]);
    
    % --- Subplot 3 (Bottom Left) ---
    ax3 = subplot(2,2,3);
    plot(T1.Time, T1.Input, 'Color', Col1, 'LineWidth', lw_b); hold on;
    plot(T2.Time, T2.Input, 'Color', Col2, 'LineWidth', lw, 'LineStyle', '--');
    plot(T3.Time, T3.Input, 'Color', Col3, 'LineWidth', lw, 'LineStyle', ':');
    ylabel('Input (V)'); xlabel('Time (s)');
    title('Control Voltage');
    yline([10 -10], 'r--', 'Limit', 'HandleVisibility', 'off'); 
    grid on;
    % Shift Down
    pos = get(ax3, 'Position'); 
    set(ax3, 'Position', [pos(1), pos(2) - shift_down, pos(3), pos(4)]);
    
    % --- Subplot 4 (Bottom Right) ---
    ax4 = subplot(2,2,4);
    plot(T1.Time, T1.Delta_Input, 'Color', Col1, 'LineWidth', lw_b); hold on;
    plot(T2.Time, T2.Delta_Input, 'Color', Col2, 'LineWidth', 1.0, 'LineStyle', '--');
    plot(T3.Time, T3.Delta_Input, 'Color', Col3, 'LineWidth', 1.0, 'LineStyle', ':');
    ylabel('\Delta u (V/s)'); xlabel('Time (s)');
    title('Input Rate');
    grid on;
    % Shift Down
    pos = get(ax4, 'Position'); 
    set(ax4, 'Position', [pos(1), pos(2) - shift_down, pos(3), pos(4)]);
    
    
    % --- HEADER LEGEND ---
    
    % 1. Create Legend
    lgd = legend([h1, h2, h3], Name1, Name2, Name3);
    set(lgd, 'Orientation', 'vertical');
    set(lgd, 'FontSize', 11);
    set(lgd, 'FontWeight', 'bold');
    set(lgd, 'Box', 'on');        % BOX IS NOW ON
    set(lgd, 'LineWidth', 1.0);   % Thicker box outline
    
    % 2. Position it in the new empty space at the top
    drawnow;
    lgdPos = get(lgd, 'Position');
    lgdWidth = lgdPos(3);
    lgdHeight = lgdPos(4);
    
    % Center X, Top Y
    newX = 0.5 - (lgdWidth / 2);
    newY = 0.96 - lgdHeight; % Place it just below the top edge
    
    set(lgd, 'Position', [newX, newY, lgdWidth, lgdHeight]);
    
    % Save
    saveas(fig, [FigName '.png']);
end