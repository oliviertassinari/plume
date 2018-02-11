target extended-remote localhost:3333
monitor reset init
define mlc
make -j9
monitor reset init
load
continue
end

