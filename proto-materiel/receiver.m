set(cstprefs.tbxprefs,'FrequencyUnits','Hz')

aR1=0;
rR=10;
aR2=10^4;
aC1=10^-8;
aC2=220*10^-12;
rL=6.5*10^-3;

s=tf('s');
H = (-(aR2*aC1*s)/((1+aR2*aC2*s)*(1+(aR1+rR)*aC1*s+rL*s^2*aC1)))
bode(H)

f1 = 1/(2*pi*aR2*aC2) % Fréquence passe bas

grid on