set(cstprefs.tbxprefs,'FrequencyUnits','Hz')

R=10^4;
C=330*10^-12;
L=20*10^-3;

s=tf('s');
H = -R/(L*s+R*C*L*s^2)
bode(H)

grid on