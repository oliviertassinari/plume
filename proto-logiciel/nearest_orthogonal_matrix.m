%compute the nearest orthogonal matrix to the given matrix

function [r]=nearest_orthogonal_matrix(m)
r=m*(m'*m)^(-1/2);
endfunction
