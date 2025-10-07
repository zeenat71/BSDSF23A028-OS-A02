SRC = src/ls-v1.0.0.c src/ls-v1.2.0.c src/ls-v1.3.0.c src/ls-v1.4.0.c src/ls-v1.5.0.c src/ls-v1.6.0.c
BIN = bin/ls-v1.0.0 bin/ls-v1.2.0 bin/ls-v1.3.0 bin/ls-v1.4.0 bin/ls-v1.5.0 bin/ls-v1.6.0

all:
	mkdir -p bin
	gcc src/ls-v1.0.0.c -o bin/ls-v1.0.0
	gcc src/ls-v1.2.0.c -o bin/ls-v1.2.0
	gcc src/ls-v1.3.0.c -o bin/ls-v1.3.0
	gcc src/ls-v1.4.0.c -o bin/ls-v1.4.0
	gcc src/ls-v1.5.0.c -o bin/ls-v1.5.0
	gcc src/ls-v1.6.0.c -o bin/ls-v1.6.0

clean:
	rm -f $(BIN)
