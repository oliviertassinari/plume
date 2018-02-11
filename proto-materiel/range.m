set(cstprefs.tbxprefs,'FrequencyUnits','Hz')

% Données
r = 1;                    % Distance e/r
f = 20*10^3;              % fréquence de fonctionnement
phi = 0;                  % Orientation relative des bobines

% Constantes physique
mu_0 = 4*pi*10^(-7);      % Permeabilité du vide
sigma = 5.96*10^7;        % Conductivité (S/m) du cuivre à 20°C

% Grandeurs
delta = 1/sqrt(mu_0*f*pi*sigma); % Epesseur de l'effet de peau (m)
s = 1i*2*pi*f;

% Emetteur
% Donnée
U_0 = 31;                 % Amplitude de la tension aux bornes du pont en H (V)
es = 0.08*10^-6;          % Section du fil (m^2)
ed = 0.315*10^-3;         % Diametre exterieure du fil (m)

eb = 0.2;                 % Epaisseur du bobinage (cm)
ea = 4-eb/2;              % Rayon moyen de la bobine (cm)
eh = 2;                   % Largeur du bobinage (cm)

eS = pi*(ea*10^-2)^2;     % Aire de la bobine (m^2)

eN = (eh*eb*10^-4)/(ed*1.419)^2; % Nombre de tour faisable dans l'espace impartie

eL = (0.31*(ea*eN)^2)/(6*ea+9*eh+10*eb)*10^(-6); % Inductance (H)
el = 2*pi*(ea*10^-2)*eN;  % Longueur de la bobine (m)
eR = el/(es*sigma);       % Résistance de la bobine en ohm en DC

% Circuit g�nerateur
gC = 1/((2*pi*f)^2*eL);   % Condition de resonnance (F)
gQ = 1/eR*sqrt(eL/gC);    % Facteur de qualité du filtre Q = w_0*L/R

gH = 1/(eR+s*eL+1/(gC*s));

I_0 = abs(gH)*U_0;         % Intensit� dans la bobine (A)

gCU = abs(1/(gH*gC*s))*U_0;
eLU = abs((eL*s)/gH)*U_0;

B_0 = (mu_0*I_0*eN*eS*sqrt(1+3*sin(phi)^2))/(4*pi*r^4);

% Recepteur
ra = 0.4;
rs = 0.008*10^-6;          % Section du fil (m^2)
rN = 250;
rS = pi*(ra*10^-2)^2;
rL = 6.5*10^-3;
rl = 2*ra*10^-2*4*rN;      % Longueur de la bobine (m)
rR = rl/(rs*sigma);             % Résistance de la bobine en ohm en DC

% Recepteur carre
ra = 0.5;                 % Rayon du carré (cm)
rb = 1;                       
rc = 0.2;

%rd = 0.15*10^-3;                % Diametre exterieur du fil (m)
%rs = 0.008*10^-6;               % Section du fil (m^2)
%rN = (rb*10^-4*rc)/(rd*1.419)^2;
%rS = (2*(ra-rc)*10^-2)^2;       % Section de la bobine (m^2)
%rl = 2*(ra-rc)*10^-2*4*rN;      % Longueur de la bobine (m)
%rR = rl/(rs*sigma);             % Résistance de la bobine en ohm en DC
%rL = 0.008*ra*rN^2*(2.303*log10(ra/(rb+rc))+0.2235*(rb+rc)/ra + 0.726)*10^-6;


% Composants de l'amplificateur
aR1 = 0;
aR2 = 10^4;
aC1 = 1/((2*pi*f)^2*rL); % Condition de resonnance (F)
aC2 = 1/(2*pi*f*3*aR2); % f1 = 3*f

aH = (-(aR2*aC1*s)/((1+aR2*aC2*s)*(1+(aR1+rR)*aC1*s+rL*s^2*aC1))); % Fonction de transf�re de l'amplificateur

% Circuit résonnant
rQ = 1/(rR+aR1)*sqrt(rL/aC1);     % Facteur de qualité du filtre

aG = abs(aH);               % Gain de l'amplificateur

e_0 = 2*pi*f*rN*rS*B_0      % Tension avant amplification (V)
s_0 = aG*e_0           % Tension apres amplification (V)
