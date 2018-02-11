%Prend une matrice des champs normalisées mesurés
%Calcule la position et l'orientation du récepteur
function [x,y,z,phi,theta,psi,e]=dof_depuis_normalise(bm)
bm;
%Calcule les normes de chacun des trois champs
nm=utils_norm(bm);
%Calcule la position a partir de ces normes dans la base de l'émetteur.
[x,y,z]=position_depuis_norme(nm(1),nm(2),nm(3));
%Calcule le champ théorique normalisé à la position théorique dans la base de l'emetteur.
bthn=champ_normalise_theorique(x,y,z);
%Calcule la matrice de rotation rotation 
R=nearest_orthogonal_matrix(bm*bthn^-1);
[phi,theta,psi]=mat_to_euler(R);
e=norm(bm-R*bthn,"fro");
endfunction
