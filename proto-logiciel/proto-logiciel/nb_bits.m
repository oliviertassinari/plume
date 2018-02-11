function [bits] =nb_bits(dmin,dmax,pp,pa)
  Bmax = champ_normalise_theorique(dmin,0,0)
  Vmax = norm(Bmax(1:3,1),"fro")
  deltaBmin = sin(pa*pi/180)*(champ_normalise_theorique(pp,dmax,0)-champ_normalise_theorique(0,dmax,0));%plus petite variation de champ que l'on souhaite mesurer
  deltaVmin = norm(deltaBmin(1:3,1),"fro")
  bits=log2(Vmax/deltaVmin);
endfunction
