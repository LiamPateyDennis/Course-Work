clc; clear; close all
%% Tutorial 2

% use 'control-enter' to run each section

%% Exercise 1
close all;
clc; clear;
s = tf('s');

G = 1/(s+1)^2;
F = 5^2/(s^2+sqrt(2)*5*s+5^2);

C = F/G;

To = minreal(zpk(C*G), 0.001)

evalfr(To, 0)

%% Exercise 2 
close all;
clc; clear;
s = tf('s');

% Affine Controller
Fq = 3^2/(s+3)^2;
G = 3/((s+3)*(s+1));
Q = minreal(Fq/G, 1e-2);
C = minreal(Q/(1-Q*G), 1e-3);

% PID gains
Kp = 7/4;
Ki = 9/6;
Kd = 5/4;
tauD = 1/6;
Cpid = minreal(Kp +Ki*1/s+Kd*(s)/(s+1/tauD), 1e-2);

T0 = zpk(minreal(Fq, 1e-1))
T0pid = zpk(minreal(G*Cpid/(1+G*Cpid)))
evalfr(T0, 0)

S0 = zpk(1-T0)
S0pid = zpk(minreal(1-T0pid, 1e-1))
evalfr(S0, 0)

Sio = zpk(1-T0)
Siopid = zpk(minreal(1-T0pid, 1e-1))
evalfr(Sio, 0)

% Rejects disturbances from constant input and output disturbances!!!

figure();
bode(T0);
hold on
bode(S0)
title('Affine Controller')
legend('T0', 'S0')

figure();
bode(T0pid);
hold on
bode(S0pid)
title('PID Controller')
legend('T0pid', 'S0pid')

% Please have a look in the following simulation
sim('tut2_2.slx')

% Please have a look at pid() function in MATLAB


%% Exercise 3
clc; clear; close all;
s = tf('s');
G = 1.8/s;
G0 = 1.8/s;
wn = 0.35;  %% BW of system

simTime= 200;
di_step  = 50;
di_fv = 0.5; % Some k constant step disturbance
% Extra: output disturbance rejection in C form (see SIMULINK)
do_step  =100;
do_fv = 0.5;

% No zero in Sio, simple controller design, no input di rejection
Fq = wn/(s+wn);
Q = Fq/G0;
Sio = evalfr(minreal((1-Fq)*G0), 0);

% Zero disturbance controller design: forcing a zero into Sio
zeta = 1/sqrt(2);
beta = sqrt(2)/0.35;
Fq1 = (beta*s+1)*wn^2/(s^2+2*zeta*wn*s+wn^2);
Q1 = Fq1/G0;
Sio = evalfr(minreal((1-Fq1)*G0), 0)

% Check bandwidth of To= Fq
BW = bandwidth(Fq1); % This may not be equal to wn and we may have to redesign Fq
if BW > 0.35
    fprintf('Bandwidth of T0 is %f', BW);
    disp('Consider re-designing Fq1 to reduce significant measurement noise \n')
else
    fprintf('Bandwidth of T0 is %f \n', BW);
    disp(':)')
end

% Traditional Controller Form
C = zpk(minreal(Q1/(1-Q1*G0)));
To = minreal(C*G0/(1+C*G0))     
So = minreal(1-To)
Sio = minreal(So*G0)
evalfr(To, 0)
evalfr(So, 0)
evalfr(Sio, 0)
%%%%%%%%%%%%%%%%% REM: MINREAL()%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Answer to question 2
sim('tut2_3');
figure();
plot(ans.y,'linewidth', 2);
hold on
plot(ans.y1,'linewidth', 2);
hold on
plot(ans.r,'linewidth', 2);
hold on
plot(ans.di,'linewidth', 2);
legend('y: No Rejection', 'y: Rejection', 'Reference', 'Input disturbance')
title('Q Form Controller')

% From the simulation we can see that the simple design of Fq does not
% reject input disturbances. 

% Notice that the error inbetween the output = 3.571.

% This error is the result of the gain of Sio(0) = 1.8/0.35 = 5.14

% For a step input disturbance of 0.5 we should expect to see 0.5*Sio(0)
% added t0 the output. Thus, we should find that:
%      y = 0.5*1.8/0.35 + ref
%      y = 2.57 + 1
%      y = 3.57
% , which is what we observed

% The rejection design of Fq was able to reject the input disturbance. The
% response to the input disturbance is characterized by Sio(s).

% Extra example including C form and the rejection of input and output
% disturbances
figure();
plot(ans.y2,'linewidth', 2);
hold on
plot(ans.r,'linewidth', 2);
hold on
plot(ans.do,'linewidth', 2);
hold on
plot(ans.di2,'linewidth', 2);
legend('y', 'ref', 'Output Disturbance', 'Input Disturbance')
title('C Form Controller')

% This is just a simulation showcasing the C-form controller, see simulink.
% Output disturbance rejection is also seen.
%% Exercise 4
clc; clear; close all;
s = tf('s');

G0 = 1/(s+1)^2;
wn = 4;

% Instead of desiging repeated poles, as in the solutions, I will instead
% design a critically damped system with  antural frequency of 4 rad/sec
zeta = 1;
Fq = 1/((s/4)^2+2*zeta*(s/4)+1);
bandwidth(Fq)
Q = Fq/G0;
C = minreal(Q/(1-Q*G0), 1e-2);

T0 = Fq;
S0 = (1-T0);
% Make sure that the reference tracks the output at steady state.
evalfr(T0, 0)
% Make sure that the controller is designed to reject the abrupt
% disturbances on the output of the system
evalfr(S0, 0)

C_inf = evalfr(C, inf)
% make sure C_feedback is STRICTLY proper
C_feedback = minreal(1/C-1/C_inf)

stepinfo(T0)

sim('tut2_4')

figure();
subplot(311)
plot(e)
hold on
plot(e1)
hold on
plot(e2)
title('Error Signal')
legend('No Saturation & No AW', 'Saturation & No AW', 'Saturation & AW')
subplot(312)
plot(u)
hold on
plot(u1)
hold on
plot(u2)
hold on
plot(1:10, 2*ones(1, 10), '-')
title('Control Signal')
legend('No Saturation & No AW', 'Saturation & No AW', 'Saturation & AW', 'Saturation')
subplot(313)
plot(y)
hold on
plot(y1)
hold on
plot(y2)
title('Output Signal')
legend('No Saturation & No AW', 'Saturation & No AW', 'Saturation & AW')
