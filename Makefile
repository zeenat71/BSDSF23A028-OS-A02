
CC = gcc
CFLAGS = -Wall -g

all: bin/ls-v1.0.0 bin/ls-v1.1.0 bin/ls-v1.2.0

# Compile v1.0.0
bin/ls-v1.0.0: src/ls-v1.0.0.c
	$(CC) $(CFLAGS) $< -o $@

# Compile v1.1.0
bin/ls-v1.1.0: src/ls-v1.1.0.c
	$(CC) $(CFLAGS) $< -o $@

# Compile v1.2.0 (Column Display)
bin/ls-v1.2.0: src/ls-v1.2.0.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f bin/*
