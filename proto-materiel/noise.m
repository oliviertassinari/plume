% Bruit à la sortie de l'ao
% On fait l'hypothese que le bruit est blanc

T = 290;
k = 1.3806*10^-23;
R_eq = 10;
e_n = 1*10^-9;
r_n = (4*k*T*R_eq)^(1/2); % Nyquist < 100Mhz
I_n = 2*10^-12;

e_t=(e_n^2+r_n^2+(I_n*R_eq)^2)^(1/2) % V/sqrt(Hz)
%s_n=Gain*e_t

Gpga = 100;
Npga = 18*10^(-9);
Glna = 10;
Nlna = 2*10^(-9);
R = 10;
B = 250;
N = 16;
FSR = 3.3;

q = FSR/(2^N-1);
Pq = q^2/12;

% Gains aux max
sqrt(Nlna^2 + (Npga^2 + Pq/(B*Gpga^2))/(Glna^2) + 4*k*T*R)*sqrt(B)

% LNA bypass PGA full
Gpga = 100;
Glna = 1;
sqrt(Nlna^2 + (Npga^2 + Pq/(B*Gpga^2))/(Glna^2) + 4*k*T*R)*sqrt(B)

% LNA bypass PGA 20db
Gpga = 10;
Glna = 1;
sqrt(Nlna^2 + (Npga^2 + Pq/(B*Gpga^2))/(Glna^2) + 4*k*T*R)*sqrt(B)

% Gain total de 1
Gpga = 1;
Glna = 1;
sqrt(Nlna^2 + (Npga^2 + Pq/(B*Gpga^2))/(Glna^2) + 4*k*T*R)*sqrt(B)