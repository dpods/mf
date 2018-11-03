
.PHONY: mf clean

mf: clean
	gcc -o ./bin/mf src/mf.c -I src/ src/matrix.c src/array.c src/str.c

test: clean
	gcc -o ./bin/test src/test.c -I src/ src/str.c

clean:
	rm -rf ./bin/mf
	rm -rf ./bin/test