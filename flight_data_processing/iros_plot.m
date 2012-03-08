clear all;

load flight1.txt;
%load flight2.txt;
%flight1 = flight2;
t_start = 160;
t_end = 200;


t_start_idx = find(flight1(:,end) > t_start);
t_end_idx = find(flight1(:,end) > t_end);
t_period = t_start_idx(1):t_end_idx(1);

figure;
plot(flight1(t_period,9),flight1(t_period,10),'.');
title('x-y location');
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
Q = 0.1*eye(6); 
R = 0.001*eye(3);
x_hat_storage = [0;0;0;0;0;0];

for i=0:length(t_period)-1
    x_hat_prev = A*x_hat;
    P_hat_prev = A*P_hat_prev*A' + Q;
    z = [flight1(t_start_idx(1) + i,9); flight1(t_start_idx(1) + i,10); flight1(t_start_idx(1) + i,11)];
    z = z*100;
    y_tilde = z - H*x_hat_prev;
    S = H*P_hat_prev*H' + R;
    K = P_hat_prev*H'*inv(S);
    x_hat = x_hat_prev + K*y_tilde;
    x_hat_storage(:,i+1) = x_hat;
end

% Plot velocity from Kalman
figure;
subplot(3,1,1);
plot(flight1(t_period,end),x_hat_storage(4,:));
title('Kalman');
subplot(3,1,2);
plot(flight1(t_period,end),vx);
title('Direct calculation');
subplot(3,1,3);
plot(flight1(t_period,end),flight1(t_period,7)*100);
title('2nd order filter');

figure;
subplot(2,1,1);
plot(flight1(t_period,end),x_hat_storage(1,:));
title('Estimated x');
subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,9)*100);
title('True x');

%%
figure;
subplot(2,1,1);
plot(flight1(t_period,end),flight1(t_period,1));
xlabel('Time [t]');
ylabel('Roll Error [deg]');
title('Roll error');

subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,2));
xlabel('Time [t]');
ylabel('Pitch Error [deg]');
title('Pitch error');

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

%% 옵틱이랑, 위치 차이로 구한 속도랑 같이 비교
figure;
subplot(2,1,1);
%plot(flight1(t_period,end),flight1(t_period,3)*10,flight1(t_period,end),[vy 0]);
plot(flight1(t_period,end),flight1(t_period,3),flight1(t_period,end),x_hat_storage(5,:));
% x_hat_storage(4,:)
xlabel('Time [t]');
ylabel('X Velocity [cm/s]');
title('Velocity comparison. Optic vs Pseudo');

subplot(2,1,2);
%plot(flight1(t_period,end),flight1(t_period,4)*10,flight1(t_period,end),[vx 0]);
plot(flight1(t_period,end),-flight1(t_period,4),flight1(t_period,end),x_hat_storage(4,:));
xlabel('Time [t]');
ylabel('Y Velocity [cm/s]');
title('Velocity comparison. Optic vs Pseudo');

%% 고도
figure;
plot(flight1(t_period,end),flight1(t_period,5),flight1(t_period,end),flight1(t_period,11)*100);
title('Altitude');

%% Zdot
figure;
plot(flight1(t_period,end),flight1(t_period,8)*100);

title('z dot');