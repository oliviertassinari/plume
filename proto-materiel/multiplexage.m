f = 200; % Frequence des positions Hz
f_0 = 20*10^3; % Frequence de fonctionnement (Hz)

R = 10;
C = 10^-8;
eTr = R*C; % Temps du régime transitoire dans la bobine émittrice

% Avec un multiplexage temporelle
N = (f_0*1/(4*f)-2*eTr)/3 % Nombre de période par mesure