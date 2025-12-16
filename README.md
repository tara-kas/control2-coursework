# COMP0211 Control Coursework

This repository contains the implementation and experimental analysis for the COMP0211 Control Systems coursework, on inverted pendulum control using PID and Model Predictive Control (MPC) approaches.

## Overview

The coursework involves:

- **PID Controller**: Manual implementation and tuning for stabilising a rotary pendulum in the upright position
- **State-Space Modelling**: Deriving and discretising the continuous-time system model from ODEs
- **MPC Controller**: Implementing constrained Model Predictive Control using YALMIP
- **Experimental Analysis**: Systematic investigation of design parameters (Q/R weighting, prediction horizon N, terminal cost P, and constraints)

## Main Files

### Controllers

- **`PID_controller.slx`**: Simulink model for PID control (calls `PID_controller.m`)
- **`PID_controller.m`**: Manual PID implementation
- **`MPC_simulink.slx`**: Simulink model for MPC control (calls `mpc_controller.m`)
- **`mpc_controller.m`**: MPC implementation with YALMIP

### Experiment Scripts

- **`run_experiments.m`**: Script to run various experimental configurations
- **`plot_experiments.m`**: Generate plots from experimental data
- **`metric_analysis.m`**: Analyse performance metrics across experiments
- **`plotting_script.m`**: Additional plotting utilities

### Data

- **`experiment_csv/`**: Contains CSV files with experimental results for different configurations (baseline, parameter variations, constraint studies)
- **`experiment_plots/`**: Generated plots from experimental analyses

## System Parameters

The coursework uses the **QUBE-Servo 3 Rotary Pendulum** with the following key parameters:

- Sampling time: 0.002 s
- Control objective: Stabilize pendulum at upright position (α = 0)
- Steady-state error requirement: α ∈ [−0.6°, 0.6°]
- Self-balance duration: ≥ 20 seconds

## Dependencies

- MATLAB/Simulink (R2024b+)
- **YALMIP**: Optimisation toolbox for MPC implementation
- **Quanser Interactive Labs (QLabs)**: For simulation and testing (optional for physical experiments)

**Note**: The `YALMIP-master/` folder is excluded from version control (see `.gitignore`). Download YALMIP from [yalmip.github.io](https://yalmip.github.io) and place it in the project root.

## Getting Started

1. **Setup YALMIP**: Download and add YALMIP to your MATLAB path
2. **Run PID Controller**: Open `PID_controller.slx` in Simulink and run
3. **Run MPC Controller**: Open `MPC_simulink.slx` in Simulink and run
4. **Run Experiments**: Execute `run_experiments.m` to generate experimental data
5. **Analyze Results**: Use `plot_experiments.m` and `metric_analysis.m` to visualize results

## Project Structure

```
├── PID_controller.slx          # PID Simulink model
├── PID_controller.m            # PID implementation
├── MPC_simulink.slx            # MPC Simulink model
├── mpc_controller.m            # MPC implementation
├── run_experiments.m           # Experiment runner
├── plot_experiments.m          # Plotting script
├── metric_analysis.m           # Performance metrics
├── experiment_csv/             # Experimental data
├── experiment_plots/           # Generated figures
└── YALMIP-master/             # YALMIP toolbox (not tracked)
```
