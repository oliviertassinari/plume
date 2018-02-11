% Param de la bobine emetrice
a = 4;             % Rayon moyen de la bobine (cm)
N = 200;           % Nombre de tour
b = 0.2            % Epaisseur du bobinage (cm)
h = 2            % Largeur du bobinage (cm))
f = 20*10^3

% Calcul de l'inductance de la bobine en microH
L = (0.31*(a*N)^2)/(6*a+9*h+10*b)*10^(-6)
% Experimentalement
% L = 3,8 mH

% Impedance de la bobine
R = 11           % Resistance en Ohm

% Condition de resonnance
% (omega)^2*L*C = 1
C = 1/((2*pi*f)^2*L)

% Facteur de qualité du filtre
Q = 1/R*sqrt(L/C)
