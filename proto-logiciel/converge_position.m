#Compute the position of the receiver
#using the norms of the 3 fields

function [x,info] = converge_position(v,ms,x0)
    disp("Entering find_position function");
    disp("v="),disp(v);
    disp("ms="),disp(ms);
    disp("x0="),disp(x0);
    norms=utils_norm(v);
    disp("norms="),disp(norms);

    #We need an anonymous function for the solver
    f = @(p) utils_norm(emitter(p,ms))-norms;
    disp("test="),disp(f(x0));
    [x,info]=fsolve(f,x0,optimset("TolFun",1e-18));
    disp("Leaving find_position function");
endfunction
