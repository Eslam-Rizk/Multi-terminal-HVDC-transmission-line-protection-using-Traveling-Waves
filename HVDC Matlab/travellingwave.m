clc; clear;
Ts=0.1e-3;
data = xlsread('Sensitivity_R12');
t=data(:,26);    %% (:,26) means the 26th column of data matrix
 
%RELAY INPUT PARAMETERS
Vdc_ss = 198.5; %steady state DC voltage (or Reference voltage)
Idc_ss = -1.03; %steady state DC Current (or Reference Current)
Zc = 23; %surge Impedance
Thr=1.5*(ones(10000,1)); %Threshold for Forward internal Fault

%THREE POINT MOVING AVERAGE FILTER
for k=2:length(data)-1
    Idc_tr(k)=(data(k-1,27)+data(k,27)+data(k+1,27))/3; % Obtain Sampled current based on moving average filter
    Vdc_tr(k)=(data(k-1,28)+data(k,28)+data(k+1,28))/3; % Obtain Sampled voltage based on moving average filter
    data_1=[Idc_tr' Vdc_tr'];% Create a Matrix for the filtered current and voltage signal
end

%STARTING UNIT
for w=2:length(data_1)-1
    derv_V(w)=(data_1(w+1,2)-data_1(w,2))/Ts;                              %could be simulated instead of trim\i_dc in protection sys. so that derv_v is dt1
    t2(w)=t(w);                                                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%
end

t2_t=t2';
dev_V=derv_V';
dv=abs(dev_V);
data_2=[t2_t dv];
[r,c]=size(data_2);
j=1;

for m=1:r
    if dv(m)>Thr %Ensures the Relay remain Stable during starting          %%% dv>1.5 %%% instead of code in fcn block
        delta_Vdc(j)=Vdc_tr(m)-Vdc_ss; %Increamental change in Voltage
        delta_Idc(j)=Idc_tr(m)-Idc_ss; %Increamental change in Current
        Pf(j)=(1/(4*Zc))*((delta_Vdc(j)^2)+(2*delta_Idc(j)*Zc*delta_Vdc(j))+(delta_Idc(j)*Zc)^2); %Power Developed by Forward Travelling Wave
        Pr(j)=(-1/(4*Zc))*((delta_Vdc(j)^2)-(2*delta_Idc(j)*Zc*delta_Vdc(j))+(delta_Idc(j)*Zc)^2); %Power Developed by Reverse Travelling Wave
        P_ratio(j)=Pf(j)/Pr(j);
        t1(j)=t2_t(m);
        Thrr(j)=Thr(j); %PROTECTION THRESHOLD
        if j==1000 %TAKE FEW SAMPLES AS PER TRAVELLING WAVE
            break
        end
    j=j+1;
    else
        delta_Vdc(j)=0;
        delta_Idc(j)=0;
        Pf(j)=0;
        Pr(j)=0;
        t1(j)=0;
        Thrr(j)=0;
    end
end

%RELAY DECISION
P_f=Pf';
P_r=Pr';
Thrr_1=Thrr';
Table=[t1' P_f P_r Thrr_1];
[rr,cc]=size(Table);

for z=1:length(Pf)
    if Pf(z)>=Thr
        disp('Fault is Forward Internal, Relay Operate')
        % else
        % disp('Healthy System, Relay is Stable')
    end
end

Table=[P_f P_r];
time=t1';
subplot(1,1,1)
plot(t1,P_f,'r',t1,P_r,'b',t1,Thrr_1,'c-.','LineWidth',2);set(gca,'fontsize',10); grid on; ylabel('TravellingWave Power (kW)','FontSize', 10);xlabel('Time(s)','FontSize', 10);
axis([2 2.01 -1E1 1E1]);legend('Forward Travelling wave power,kW','Backward Travelling wave power,kW','Threshold for Forward Internal Fault','Location','northeast','Orientation','vertical');text(2.501,2e4 ,'Relay R1','Color','k','FontSize',10)
%% Determine Energy Contents of the forward and reverse Travelling Wave.
SS=size(Pf);
for z=1:SS
    Ef_tra=0.0001*(0.5*(Pf(1)+Pf(end))+sum(Pf)-Pf(1)- Pf(end));%Trapezoidal Algorithm
    A=sum(Pf(2:2:length(Pf)))*2;
    B=sum(Pf(1:2:length(Pf)))*4;
    C=1.04e-5/3;
    Ef_symp=C*(A+B-Pf(1));%Simpson's Algorithm
end
ST=size(Pr);
for z=1:ST
    Er_tra=0.00001*(0.5*(Pr(1)+Pr(end))+sum(Pr)-Pr(1)- Pr(end));%Trapezoidal Algorithm
    A=sum(Pr(2:2:length(Pr)))*2;
    B=sum(Pr(1:2:length(Pr)))*4;
    C=1.04e-5/3;
    Er_symp=C*(A+B-Pr(1));%Simpson's Algorithm
end