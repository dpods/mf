
.PHONY: mf mf2 clean

mf: clean
	gcc -o ./bin/mf src/mf.c -I src/ src/matrix.c src/array.c src/str.c

mf2: clean
	gcc -o ./bin/mf2 src2/mf2.c -I src2/ src2/matrix.c src2/str.c

clean:
	rm -rf ./bin/mf
	rm -rf ./bin/mf2