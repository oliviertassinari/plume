function [c1,c2,p0,v,y,info]=calibration(c1i,c2i,pi,vui,m0,t1,m1,t2,m2,t3,m3,t4,m4,t5,m5)
%Vecteur initial pour la convergence
%Correspond a la concatenation horizontale de c1,c2,p0,v
x0=horzcat(c1i,c2i,pi,vui)

%Fonction anonyme à minimiser
f=@(x) sum([a_minimiser(0,m0,x),a_minimiser(t1,m1,x),a_minimiser(t2,m2,x),a_minimiser(t3,m3,x),a_minimiser(t4,m4,x),a_minimiser(t5,m5,x)]);

%minimize
[xs,y,info]=fminunc(f,x0,optimset("MaxFunEvals",10000000,"MaxIter",10000000,"TolX",1e-60,"TolFun",1e-60));

c1=diag(xs(1:3,1));
c2=diag(xs(1:3,2));
p0=xs(1:3,3);
v=xs(1:3,4);
endfunction
