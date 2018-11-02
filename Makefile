
.PHONY: mf clean

mf: clean
	gcc -o ./bin/mf src/mf.c -I src/ src/matrix.c src/array.c

clean:
	rm -rf ./bin/mf