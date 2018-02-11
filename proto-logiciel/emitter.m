#compute the 9 field values at a given position

function [v] = emitter(p,ms)
    disp("Entering emitter function");
    disp("p="),disp(p);
    disp("ms="),disp(ms);
   v=[dipole(p,ms(:,1)),dipole(p,ms(:,2)),dipole(p,ms(:,3))];
   disp("Leaving emitter function");
endfunction
