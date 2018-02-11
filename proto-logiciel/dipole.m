#compute in position p the magnetic field for a 
#magnetic dipole of momentum m 

function [b] = dipole(p,g)
    disp("Entering dipole function");
    disp("p="),disp(p);
    disp("g="),disp(g);
    q=norm(p);
    n=p/q;
    b=(3*dot(n,g)*n-g)/q^3;
    disp("Leaving dipole function");
endfunction
