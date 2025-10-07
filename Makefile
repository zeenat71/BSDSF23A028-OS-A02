SRC = src/ls-v1.0.0.c src/ls-v1.2.0.c src/ls-v1.3.0.c src/ls-v1.4.0.c src/ls-v1.5.0.c src/ls-v1.6.0.c
OBJ = obj/ls-v1.0.o obj/ls-v1.2.o obj/ls-v1.3.o obj/ls-v1.4.o obj/ls-v1.5.o obj/ls-v1.6.o
BIN = bin/ls-v1.0.0 bin/ls-v1.2.0 bin/ls-v1.3.0 bin/ls-v1.4.0 bin/ls-v1.5.0 bin/ls-v1.6.0

all:
	mkdir -p bin obj
	gcc -c src/ls-v1.0.0.c -o obj/ls-v1.0.o
	gcc -c src/ls-v1.2.0.c -o obj/ls-v1.2.o
	gcc -c src/ls-v1.3.0.c -o obj/ls-v1.3.o
	gcc -c src/ls-v1.4.0.c -o obj/ls-v1.4.o
	gcc -c src/ls-v1.5.0.c -o obj/ls-v1.5.o
	gcc -c src/ls-v1.6.0.c -o obj/ls-v1.6.o
	gcc obj/ls-v1.0.o -o bin/ls-v1.0.0
	gcc obj/ls-v1.2.o -o bin/ls-v1.2.0
	gcc obj/ls-v1.3.o -o bin/ls-v1.3.0
	gcc obj/ls-v1.4.o -o bin/ls-v1.4.0
	gcc obj/ls-v1.5.o -o bin/ls-v1.5.0
	gcc obj/ls-v1.6.o -o bin/ls-v1.6.0

clean:
	rm -f obj/* bin/*
