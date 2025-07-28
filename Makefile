waxc: src/waxc.c
	gcc src/waxc.c -o waxc -O3 -std=c99 -pedantic -Wall -std=c23

c: _
	gcc -g src/waxc.c -DEBUG -o waxc

text: _
	cd tools; python3 concat.py; cd ../
