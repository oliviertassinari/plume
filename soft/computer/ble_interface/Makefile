
.phony: install clean jsclean

install:
	npm i
	cp lib/my_bindings.js node_modules/noble/lib/linux/bindings.js

clean:
	rm -rf node_modules

jsclean: clean
	rm -f *.js
	rm -f *.map
