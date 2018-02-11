cap = load("capture.txt");
for(i = 1:3)
  figure(i);
  subplot(3,1,1);
  plot(cap(:,i));
  subplot(3,1,2);
  plot(cap(:,3+i));
  subplot(3,1,3);
  plot(cap(:,i).*10.^(-cap(:,3+i)/40));
endfor

figure(4);
subplot(3,1,1);
plot(40*log10(cap(:,1:3)));
subplot(3,1,2);
plot(cap(:,4:6));
subplot(3,1,3);
plot(40*log10(cap(:,1:3))-cap(:,4:6));

