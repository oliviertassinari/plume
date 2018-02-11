function [rx,ry,rz]=step(bm,x,y,z)

p=[x,y,z];
r=norm(p);
bnth=r^-5*(3*(p'*p)-r^2*eye(3))
rt=nearest_orthogonal_matrix(bnth*bm^-1)
c=projection_champ(rt*bm,bnth,x,y,z)

%compute the by-column norms
n=[norm(c(:,1),"fro");norm(c(:,2),"fro");norm(c(:,3),"fro")]
s=(6/norm(n,"fro")^2)^(1/6)

%compute rx ry rz
rx=sqrt(((s^6*n(1)^2-1)*s^2)/3);
ry=sqrt(((s^6*n(2)^2-1)*s^2)/3);
rz=sqrt(((s^6*n(3)^2-1)*s^2)/3);

endfunction

