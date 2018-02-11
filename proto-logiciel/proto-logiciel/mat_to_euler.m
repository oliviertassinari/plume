function [theta,phi,psi]=mat_to_euler(R)
	 R
         theta=180/pi*atan2(R(2,1),R(1,1));
	 phi=180/pi*asin(R(3,1));
	 psi=180/pi*atan2(R(3,2),R(3,3));
endfunction
