% --- Save_Run_to_CSV.m ---
% Extracts data from 'out' and saves it to the 'experiments_csv' folder.

% 1. Extract Data from the 'out' container
t_data = out.tout;
theta_data = out.data_theta; 
alpha_data = out.data_alpha;
u_data = out.data_u;

% 2. Define the Folder Name
folder_name = 'experiment_csv';

% Check if the folder exists; if not, create it automatically
if ~exist(folder_name, 'dir')
    mkdir(folder_name);
end

% 3. Ask for a Filename
run_name = input('Enter name for this file: ', 's');

% --- NEW: Construct the full path ---
% fullfile combines the folder and filename safely
filename = fullfile(folder_name, strcat(run_name, '.csv'));

% 4. Create a Table
Data = table(t_data, theta_data, alpha_data, u_data, ...
    'VariableNames', {'Time', 'Theta', 'Alpha', 'Input'});

% 5. Write to CSV file in the specific folder
writetable(Data, filename);

fprintf('SUCCESS: Data saved to "%s".\n', filename);