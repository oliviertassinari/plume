fid = fopen("buffer.bin","r","native");
buf = fread(fid,"int32","native");
l = buf(1:2:size(buf));
r = buf(2:2:size(buf));
hold off;
plot(l);
hold on
plot(r,'r');
plot(sqrt(l.^2 + r.^2),'g');
