CC = gcc
CFLAGS = -Wall -g

# Directories
SRC_DIR = src
BIN_DIR = bin

# Files
SRC_V1_2 = $(SRC_DIR)/ls-v1.2.0.c
BIN_V1_2 = $(BIN_DIR)/ls-v1.2.0

all: $(BIN_V1_2)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_V1_2): $(SRC_V1_2) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_V1_2) -o $(BIN_V1_2)

clean:
	rm -rf $(BIN_DIR)/*
