clear all;
close all;

load flight1.txt;
%load flight2.txt;
%flight1 = flight2;
%t_start = 160;
%t_end = 210;
t_start = 160;
t_end = 210;


t_start_idx = find(flight1(:,end) > t_start);
t_end_idx = find(flight1(:,end) > t_end);
t_period = t_start_idx(1):t_end_idx(1);
%
figure;
plot(flight1(t_period,9) - flight1(t_period(1),9),flight1(t_period,10) - flight1(t_period(1),10),'k');
title('X-Y location (top view)'); axis equal; grid on;
axis([-1 1 -1 1]);
xlabel('X axis [m]');
ylabel('Y axis [m]');
print('-dpdf','xy_topview_fig');

figure;
subplot(3,1,1);
plot(flight1(t_period,end) - t_start,flight1(t_period,9)- flight1(t_period(1),9),'k');
title('X location'); xlabel('Time [s]'); ylabel('Distance [m]'); grid on;
xlim([0 t_end - t_start]); ylim([-1 1]);
subplot(3,1,2);
plot(flight1(t_period,end) - t_start,flight1(t_period,10)- flight1(t_period(1),10),'k');
title('Y location'); xlabel('Time [s]'); ylabel('Distance [m]'); grid on;
xlim([0 t_end - t_start]); ylim([-1 1]);
subplot(3,1,3);
plot(flight1(t_period,end) - t_start,flight1(t_period,11),'k');
title('Z location'); xlabel('Time [s]'); ylabel('Distance [m]'); grid on;
xlim([0 t_end - t_start]); ylim([-1 1]);
print('-dpdf','xyz_loc_fig');
%% Velocity calc.
for i=2:length(t_period)+1
    dx = flight1(i + t_start_idx(1),9) - flight1(i - 1 + t_start_idx(1),9);
    dy = flight1(i + t_start_idx(1),10) - flight1(i - 1 + t_start_idx(1),10);
    vx(i-1) = dx / 0.01 * 100;
    vy(i-1) = dy / 0.01 * 100;
end


%% Kalman filter to estimate velocity.
dt = 0.01;

A = [eye(3) dt*eye(3);
     zeros(3,3) eye(3)];

H = [eye(3) zeros(3,3)];

x_hat = [flight1(t_start_idx(1),9);flight1(t_start_idx(1),10);flight1(t_start_idx(1),11);0;0;0];  % initial state
P_hat = eye(6);
P_hat_prev = eye(6);
x_hat_prev = [0;0;0;0;0;0];  % initial state
Q = 100*eye(6);  % Process noise
R = 0.1*eye(3); % Measurement noise
x_hat_storage = [0;0;0;0;0;0];
y_tilde_storage = [0;0;0];

for i=0:length(t_period)-1
    x_hat_prev = A*x_hat;
    P_hat_prev = A*P_hat*A' + Q;
    z = [flight1(t_start_idx(1) + i,9); flight1(t_start_idx(1) + i,10); flight1(t_start_idx(1) + i,11)];
    z = z*100;
    y_tilde = z - H*x_hat_prev;
    S = H*P_hat_prev*H' + R;
    K = P_hat_prev*H'*inv(S);
    x_hat = x_hat_prev + K*y_tilde;
    P_hat = (eye(6) - K*H)*P_hat_prev;
    x_hat_storage(:,i+1) = x_hat;
    y_tilde_storage(:,i+1) = y_tilde;
end

close all;
% Plot velocity from Kalman
figure;
subplot(3,1,1);
plot(flight1(t_period,end),x_hat_storage(4,:)); grid on;
title('Kalman');
subplot(3,1,2);
plot(flight1(t_period,end),vx); grid on;
title('Direct calculation');
subplot(3,1,3);
plot(flight1(t_period,end),flight1(t_period,7)*100); grid on;
title('2nd order filter');

figure;
subplot(2,1,1);
plot(flight1(t_period,end),x_hat_storage(1,:)); grid on;
title('Estimated x');
subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,9)*100); grid on;
title('True x');

figure;
subplot(3,1,1);
plot(flight1(t_period,end),y_tilde_storage(1,:)); grid on;
title('X tilde');
subplot(3,1,2);
plot(flight1(t_period,end),y_tilde_storage(2,:)); grid on;
title('Y tilde');
subplot(3,1,3);
plot(flight1(t_period,end),y_tilde_storage(3,:)); grid on;
title('Z tilde');

%% 옵틱이랑, Kalman filter로 구한 속도랑 같이 비교
close all;
figure;
subplot(2,1,1);
plot(flight1(t_period,end) - t_start,flight1(t_period,3)*10,'k'); hold on;
plot(flight1(t_period,end) - t_start,-x_hat_storage(4,:),'k','LineWidth',2.0); grid on;
xlabel('Time [s]');
ylabel('X Velocity [cm/s]');
xlim([0 t_end - t_start]); ylim([-100 100]);
title('Velocity of X-axis');
legend('Optical flow sensor','Vicon');

subplot(2,1,2);
plot(flight1(t_period,end) - t_start,-flight1(t_period,4)*10,'k'); hold on;
plot(flight1(t_period,end) - t_start,x_hat_storage(5,:),'k','LineWidth',2.0); grid on;
xlabel('Time [s]');
ylabel('Y Velocity [cm/s]');
xlim([0 t_end - t_start]); ylim([-100 100]);
title('Velocity of Y-axis');
legend('Optical flow sensor','Vicon');

print('-dpdf','velocity_comparison');

%%
close all;
figure;
subplot(2,1,1);
plot(flight1(t_period,end) - t_start,flight1(t_period,1),'k'); grid on;
xlabel('Time [s]');
xlim([0 t_end - t_start]);
ylim([-5 5]);
ylabel('Roll Error [deg]');
title('Roll error');

subplot(2,1,2);
plot(flight1(t_period,end) - t_start,flight1(t_period,2),'k'); grid on;
xlabel('Time [s]');
xlim([0 t_end - t_start]);
ylim([-5 5]);
ylabel('Pitch Error [deg]');
title('Pitch error');

print('-dpdf','attitude_error');

%% 옵틱이랑, 바이컨 에서, 2차필터로 얻은 속도랑 비교
figure;
subplot(2,1,1);
plot(flight1(t_period,end),flight1(t_period,3)*10,flight1(t_period,end),flight1(t_period,7)*100);
xlabel('Time [t]');
ylabel('X Velocity [cm/s]');
title('Velocity view');

subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,4)*10,flight1(t_period,end),flight1(t_period,6)*100);
xlabel('Time [t]');
ylabel('Y Velocity [cm/s]');
title('Velocity view');

%%

plot(flight1(t_period,end),flight1(t_period,7)*100,flight1(t_period,end),[vx 0]);
legend('real','pseudo');
title('Velocity comparison. Pseudo, and 2nd order filter');

%% 고도
figure;
plot(flight1(t_period,end),flight1(t_period,5),flight1(t_period,end),flight1(t_period,11)*100);
title('Altitude');

%% Zdot
figure;
plot(flight1(t_period,end),flight1(t_period,8)*100);

title('z dot');