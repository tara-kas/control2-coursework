% --- Save_Run_to_CSV.m ---
% Extracts data from 'out' and saves it to the 'experiments_csv' folder.

% 1. Extract Data from the 'out' container
t_data = out.tout;
theta_data = out.data_theta; 
alpha_data = out.data_alpha;
u_data = out.data_u;

folder_name = 'experiment_csv';
if ~exist(folder_name, 'dir')
    mkdir(folder_name);
end
run_name = input('Enter name for this file: ', 's');


filename = fullfile(folder_name, strcat(run_name, '.csv'));
Data = table(t_data, theta_data, alpha_data, u_data, ...
    'VariableNames', {'Time', 'Theta', 'Alpha', 'Input'});

writetable(Data, filename);

fprintf('SUCCESS: Data saved to "%s".\n', filename);