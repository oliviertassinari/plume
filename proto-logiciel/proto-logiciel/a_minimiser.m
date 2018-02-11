function [y]=a_minimiser(t,m,x)
    c1=diag(x(1:3,1));
    c2=diag(x(1:3,2));
    p0=x(1:3,3);
    vu=x(1:3,4);
    theta=vu(1);
    phi=vu(2);
    v=[cos(theta)*cos(phi);sin(theta)*cos(phi);sin(phi)];
    c1(1,1)=1;
    [a,b,c]=dof_depuis_normalise(c1*m*c2);
    y=norm([a;b;c]-(p0+t*v),"fro")^2;
endfunction
