% --- Plot Horizon (N) Experiments (2x2 Grid + Centered Legend) ---
% Compares N = 5, 10, 30, 50, 80
% Features: Distinct colors, vertical centered legend, publication layout.

clear; clc; close all;

% --- 1. Load Data ---
% Define files and their display names/colors
experiments = {
    'N5.csv',       'N=5 (Short)',      [0.85, 0.33, 0.1]; % Burnt Orange
    'Baseline.csv', 'N=10 (Baseline)',  'k';               % Black
    'N30.csv',      'N=30',             'b';               % Blue
    'N50.csv',      'N=50',             [0, 0.6, 0];       % Dark Green
    'N80.csv',      'N=80 (Long)',      'm'                % Magenta
};

data = containers.Map();

for i = 1:size(experiments, 1)
    fname = experiments{i, 1};
    if isfile(fname)
        T = readtable(fname);
        ts = 0.002;
        T.Delta_Input = [0; diff(T.Input) ./ ts];
        data(fname) = T;
    else
        warning('File %s not found!', fname);
    end
end

% --- 2. Create Figure ---
create_horizon_figure(data, experiments);

function create_horizon_figure(dataMap, expList)
    
    % Setup Figure
    fig = figure('Name', 'Effect of Prediction Horizon N', 'Color', 'w', 'Position', [100, 50, 1000, 950]);
    lw = 1.2; % Line width
    
    % Prepare Subplots
    ax1 = subplot(2,2,1); grid on; hold on; ylabel('\theta (deg)'); xlabel('Time (s)'); title('Arm Angle');
    ax2 = subplot(2,2,2); grid on; hold on; ylabel('\alpha (deg)'); xlabel('Time (s)'); title('Pendulum Angle'); yline(0, 'k--', 'HandleVisibility', 'off');
    ax3 = subplot(2,2,3); grid on; hold on; ylabel('Input (V)'); xlabel('Time (s)'); title('Control Voltage'); yline([10 -10], 'r--', 'HandleVisibility', 'off');
    ax4 = subplot(2,2,4); grid on; hold on; ylabel('\Delta u (V/s)'); xlabel('Time (s)'); title('Input Rate');
    
    % Loop through experiments to plot
    handles = [];
    names = {};
    
    for i = 1:size(expList, 1)
        fname = expList{i, 1};
        name  = expList{i, 2};
        col   = expList{i, 3};
        
        if isKey(dataMap, fname)
            T = dataMap(fname);
            
            % Plot on all axes
            h = plot(ax1, T.Time, T.Theta_2, 'Color', col, 'LineWidth', lw);
            plot(ax2, T.Time, T.Alpha,   'Color', col, 'LineWidth', lw);
            plot(ax3, T.Time, T.Input,   'Color', col, 'LineWidth', lw);
            plot(ax4, T.Time, T.Delta_Input, 'Color', col, 'LineWidth', lw);
            
            % Store handle for legend
            handles = [handles, h];
            names{end+1} = name;
        end
    end
    
    % --- Shift Grids Down for Legend Space ---
    axes_list = [ax1, ax2, ax3, ax4];
    shift_down = 0.08;
    for ax = axes_list
        pos = get(ax, 'Position');
        set(ax, 'Position', [pos(1), pos(2) - shift_down, pos(3), pos(4)]);
    end
    
    % --- Create Floating Legend ---
    lgd = legend(handles, names, 'Orientation', 'horizontal');
    set(lgd, 'FontSize', 10, 'FontWeight', 'bold', 'Box', 'on', 'LineWidth', 1);
    
    drawnow;
    lgdPos = get(lgd, 'Position');
    newX = 0.5 - (lgdPos(3) / 2);
    newY = 0.96 - lgdPos(4);
    set(lgd, 'Position', [newX, newY, lgdPos(3), lgdPos(4)]);
    
    saveas(fig, 'Horizon_Experiment.png');
    fprintf('Saved "Horizon_Experiment.png"\n');
end