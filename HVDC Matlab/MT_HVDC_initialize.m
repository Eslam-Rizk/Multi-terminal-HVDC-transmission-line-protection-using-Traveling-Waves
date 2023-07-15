%Script for 21-level MMC HVDC Back-To-Back DC Link converter:
clc
%Initial time constants:
Ts_Power =20e-6;                            %Power System sampling time.
Ts_Control=40e-6;                           %Control system sampling time.
Fc = 50;                                    %Switching frequency of semi-conductors.
%% Grid Parameters:
f = 50;                                     %grid frequency.
w = 2*pi*f;                                 %angular frequency.

V_prim = 400e3;                             %Primary dist grid.
V_grid = 66e3;                              %RMS L-L Voltage of B2B.

Skk = 12000e6;                              %Maximum short circuit power of source
xr_ratio = 14.28;                           %X/R ratio of grid.
z_int_400 = sqrt(3)*(V_prim)^2/Skk;         %Base impedance in short circuit of source.
R_int = sqrt((z_int_400^2)/(1+xr_ratio^2)); %Internal Resistance source.
X_int = R_int*xr_ratio;                     %Internal reactance source. 
L_int = X_int/w;                            %Internal inductance source.
z0z1 = 0.8;                                 %Zero sequence impedance

%% Load Parameters:
P_nom = 220e6;                              %Nominal power of converters.
S_nom = 220e6;                              %Nominal apparent power of converters.
I_nom = S_nom/(sqrt(3)*V_grid);             %Nominal line current.
P_load = 110e6;                             %Load active power nominal.
Q_load = 90e6;                               %Load reative power nominal.
S_load = sqrt(P_load^2 + Q_load^2);         %Apparent load power
R_startup = 17;                             %Startup resistance for inrush.

P_DynLoad = 100e6;                          %Non-ideal Load Active Power.
Q_DynLoad = 50e6;                           %Non-ideal Load Reactive Power.

%%
%%TRANSFORMER 1:
V1_prim = V_prim;
V1_sec = V_grid;
S1_nom = P_nom;
L1_pu = 0.109;
R1_pu = 0.00206;
Rg_T1 = 40;
%%
%%TRANSFORMER 2:
V2_prim = V_grid;
V2_sec = 18e3;
S2_nom = P_nom;
L2_pu = 0.0824;
R2_pu = 0.00206;

%% DC Link Parameters:
Vdc_nom = 135e3;                            %Rated DC voltage.

%% Submodule Parameters - Rectifier and Inverter:
n = 50;                                     %Number of submodules per arm, Rectifier.
n2 = 50;                                    %Number of submodules per arm, Inverter.
C_pm = 10.48e-3;                            %Submodule capacitor capacitance.
Vc_pm_i = 0*Vdc_nom/n2;                     %Initial SM capacitor voltage inverter.
Vc_pm_r = 0*Vdc_nom/n;                      %Initial SM capacitor voltage rectifier.

%% Converter arms - Rectifier and Inverter:
R_arm = 0.03;                               %Arm resistance.
L_arm_pu = 0.15;                            %Arm inductance in per-unit.
Z_base66 = (sqrt(3)*(V_grid)^2)/(S_nom);    %Network impedance.
L_arm = L_arm_pu*Z_base66/w;                %Arm inductance.
R_arm_pu = R_arm/Z_base66;                  %Arm resistance in per-unit.


%% Controller settings
%%Rectifier Parameters:

Kp_Ireg_r = 2;                              %Parameter of Inner current control.
Ki_Ireg_r = 100;                            %Parameter of Inner current control.
Ireg_lim_r = [1.5, -1.5];                   %Limit control for current loop.

Kp_DCreg_r = 8;                             %Outer voltage controller for rectifier.                         
Ki_DCreg_r = 150;                           %Outer voltage controller for rectifier.
DC_lim = [1.05 -1.05];                      %Limits for DC regulator (Maximum current allowed).

%%Inverter Parameters:
Kp_Ireg_i = 2;                              %Parameter of Inner current control.
Ki_Ireg_i = 200;                            %Parameter of Inner current control.
Ireg_lim_i = [1.5, -1.5];                   %Limit control for current loop.

V_ac_ref = 1.035;                           %voltage reference for load in p.u.
Kp_ACreg_i = 2;                             %Outer voltage controller for rectifier.
Ki_ACreg_i = 260;                           %Outer voltage controller for rectifier.
AC_lim = [1.5 -1.5];                        %Limits for AC voltage regulator.


%%Zero Sequence Controller (parameters for PR controller in loop:
Kp_R_inv = 0.001;
Ki_R_inv = 150000;

Kp_R_rec = 0.001;
Ki_R_rec = 5000;

%%CCSC: (parameters for PI controllers, common for inv. and rectifier).
Kp_ccsc = 1;
Ki_ccsc = 5;
ccsc_lim = [1.0 -1.0];

% Filters for control signals in dq0;
Fn_filter=1000;                             %cutoff frequency for second order filter, for control system.
Zeta_filter=1;                              %damping ratio of second order filter, for control system.
%% Fault scenario
T_fault = 1.4;                              %Enables voltage modification from source (dip)
