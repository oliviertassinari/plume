function [x,y,z]=position_depuis_norme(bx,by,bz)
    r=(6/(bx*bx+by*by+bz*bz))^(1/6);
    x=abs(sqrt((bx*bx*r^8-r^2)/3));
    y=abs(sqrt((by*by*r^8-r^2)/3));
    z=abs(sqrt((bz*bz*r^8-r^2)/3));
endfunction

