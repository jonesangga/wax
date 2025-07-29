waxc: src/waxc.c src/to_c.c src/common.c
	gcc src/waxc.c -o waxc -std=c99 -pedantic -Wall -std=c23

.PHONY: test
test:
	@cd tests; \
	bash test.sh ../waxc; \
	cd ..

c: _
	gcc -g src/waxc.c -DEBUG -o waxc

text:
	cd tools; python3 concat.py; cd ../
