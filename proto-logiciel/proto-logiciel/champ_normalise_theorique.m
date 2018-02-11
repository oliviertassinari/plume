%calcule le champ normalisé théorique.
%dans le repère de l'émetteur
function [bthn] = champ_normalise_theorique(x,y,z)
    r=sqrt(x*x+y*y+z*z);
    bthn=(r)^(-5)*[3*x*x-r*r,3*x*y,3*x*z;3*x*y,3*y*y-r*r,3*y*z;3*x*z,3*y*z,3*z*z-r*r];
endfunction
