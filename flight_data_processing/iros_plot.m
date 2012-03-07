clear all;

%load flight1.txt;
load flight2.txt;
flight1 = flight2;
t_start = 160;
t_end = 200;


t_start_idx = find(flight1(:,end) > t_start);
t_end_idx = find(flight1(:,end) > t_end);
t_period = t_start_idx:t_end_idx;

figure;
plot(flight1(t_period,9),flight1(t_period,10),'.');
title('x-y location');
%% Velocity calc.
for i=2:length(t_period)
    dx = flight1(i + t_start_idx(1),9) - flight1(i - 1 + t_start_idx(1),9);
    dy = flight1(i + t_start_idx(1),10) - flight1(i - 1 + t_start_idx(1),10);
    vx(i-1) = dx / 0.01 * 100;
    vy(i-1) = dy / 0.01 * 100;
end

%%
figure;
subplot(2,1,1);
plot(flight1(t_period,end),flight1(t_period,1));
xlabel('Time [t]');
ylabel('Roll Error [deg]');

subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,2));
xlabel('Time [t]');
ylabel('Pitch Error [deg]');

%% 옵틱이랑, 바이컨 에서, 2차필터로 얻은 속도랑 비교
figure;
subplot(2,1,1);
plot(flight1(t_period,end),flight1(t_period,3)*10,flight1(t_period,end),flight1(t_period,7)*100);
xlabel('Time [t]');
ylabel('X Velocity [cm/s]');

subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,4)*10,flight1(t_period,end),flight1(t_period,6)*100);
xlabel('Time [t]');
ylabel('Y Velocity [cm/s]');

%%

plot(flight1(t_period,end),flight1(t_period,7)*100,flight1(t_period,end),[vx 0]);
legend('real','pseudo');

%% 옵틱이랑, 위치 차이로 구한 속도랑 같이 비교
figure;
subplot(2,1,1);
plot(flight1(t_period,end),flight1(t_period,3)*10,flight1(t_period,end),[vy 0]);
xlabel('Time [t]');
ylabel('X Velocity [cm/s]');

subplot(2,1,2);
plot(flight1(t_period,end),flight1(t_period,4)*10,flight1(t_period,end),[vx 0]);
xlabel('Time [t]');
ylabel('Y Velocity [cm/s]');

%% 고도
figure;
plot(flight1(t_period,end),flight1(t_period,5),flight1(t_period,end),flight1(t_period,11)*100);

%% Zdot
figure;
plot(flight1(t_period,end),flight1(t_period,8)*100);