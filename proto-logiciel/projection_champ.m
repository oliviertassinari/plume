function [c]=projection_champ(b,bnth,x,y,z)

%calcul des dérivés en x y z
dbx=x*[4*x,3*y,3*z;3*y,-2*x,0;3*z,0,-2*x];
dbx=dbx/norm(dbx,"fro")
dby=y*[-2*y,3*x,0;3*x,4*y,3*z;0,3*z,-2*y];
dby=dby/norm(dby,"fro")
dbz=z*[-2*z,0,3*x;0,-2*z,3*y;3*x,3*y,4*z];
dbz=dbz/norm(dbz,"fro")


c=bnth+dot((b-bnth),dbx)*dbx+dot((b-bnth),dby)*dby+dot((b-bnth),dbz)*dbz;

endfunction
