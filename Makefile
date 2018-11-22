
.PHONY: mf clean

mf: clean
	gcc -o ./bin/mf src/mf.c -I src/ src/matrix.c src/str.c

clean:
	rm -rf ./bin/mf