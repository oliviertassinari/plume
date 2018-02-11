function [y] = utils_norm(x)
    y=[norm(x(:,1),"fro");norm(x(:,2),"fro");norm(x(:,3),"fro")];
endfunction
