pkg load signal;

Fs = 192000; # sampling frequency
Fe = 20000; # emitter frequency
T_emit = 0.00125; # time of emission
T_stab = 0.00025; # stabilisation time
A = -5;

N=T_emit*Fs;

input = A*sin(Fe/Fs*2*pi*[0:N-1]).*(1-exp(-1/N*[0:N-1]*5*T_emit/T_stab));
figure(1);
hold off;
plot(T_emit/N*[0:N-1],input);
hold on;
input_noisy = input + 1*randn(1,N);
plot(T_emit/N*[0:N-1],input_noisy);

[passe_bande_b,passe_bande_a]=butter(3,[(Fe-500)/Fs,(Fe+500)/Fs]);
input_filtered = filter(passe_bande_b,passe_bande_a,input_noisy);

S=sin(Fe/Fs*2*pi*[0:N-1]);
C=cos(Fe/Fs*2*pi*[0:N-1]);

input_filtered_s = input_filtered.*S;
input_filtered_c = input_filtered.*C;

[b,a]=butter(3,0.01);
input_s_filtered = filter(b,a,input_filtered_s);
input_c_filtered = filter(b,a,input_filtered_c);

figure(2);
subplot(3,1,1);
hold off;
plot(T_emit/N*[0:N-1],input_filtered_s);
hold on;
plot(T_emit/N*[0:N-1],input_s_filtered,'r');

subplot(3,1,2);
hold off;
plot(T_emit/N*[0:N-1],input_filtered_c);
hold on;
plot(T_emit/N*[0:N-1],input_c_filtered,'r');

subplot(3,1,3);
hold off;
plot(T_emit/N*[0:N-1],(input_c_filtered.^2+input_s_filtered.^2).^.5);

N_stab = T_stab*Fs;
outc = mean(input_filtered_c(N_stab+2:N-2))
outs = mean(input_filtered_s(N_stab+2:N-2))
out = (outc^2+outs^2)^.5
phase = atan2(outc,outs)
