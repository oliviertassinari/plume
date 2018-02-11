%compute the distance using the 4-matrix method
%found in Etude.pdf

function [r,x,y,z]=compute_distance(g,omega,v,s)
%compute the product of the matrices
m=inv(g)*inv(omega)*v.*inv(s);

%compute the radius
r=(6^(1/2)/norm(m,"fro"))^(1/3);

%compute the by-column norms
n=[norm(m(:,1),"fro");norm(m(:,2),"fro");norm(m(:,3),"fro")];

%compute x
x=sqrt((r^6*n(1)^2-1)*r^2/3);
y=sqrt((r^6*n(2)^2-1)*r^2/3);
z=sqrt((r^6*n(3)^2-1)*r^2/3);

endfunction
