function [bmn]=champ_normalise_depuis_tension(g,omega,v,s)
    bnm=inv(g)*inv(omega)*v.*ins(s);
endfunction
