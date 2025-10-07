# Makefile for all ls versions

SRC_DIR = src
BIN_DIR = bin

SRC = $(SRC_DIR)/ls-v1.0.0.c \
      $(SRC_DIR)/ls-v1.2.0.c \
      $(SRC_DIR)/ls-v1.3.0.c \
      $(SRC_DIR)/ls-v1.4.0.c

BIN = $(BIN_DIR)/ls-v1.0.0 \
      $(BIN_DIR)/ls-v1.2.0 \
      $(BIN_DIR)/ls-v1.3.0 \
      $(BIN_DIR)/ls-v1.4.0

# Default target
all: $(BIN_DIR) $(BIN)

# Create bin folder if not exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile each source to its binary
$(BIN_DIR)/ls-v1.0.0: $(SRC_DIR)/ls-v1.0.0.c | $(BIN_DIR)
	gcc $< -o $@

$(BIN_DIR)/ls-v1.2.0: $(SRC_DIR)/ls-v1.2.0.c | $(BIN_DIR)
	gcc $< -o $@

$(BIN_DIR)/ls-v1.3.0: $(SRC_DIR)/ls-v1.3.0.c | $(BIN_DIR)
	gcc $< -o $@

$(BIN_DIR)/ls-v1.4.0: $(SRC_DIR)/ls-v1.4.0.c | $(BIN_DIR)
	gcc $< -o $@

# Clean binaries
clean:
	rm -rf $(BIN_DIR)/*

SRC_V1_5 = src/ls-v1.5.0.c
BIN_V1_5 = bin/ls-v1.5.0

v1.5.0:
	mkdir -p bin
	gcc $(SRC_V1_5) -o $(BIN_V1_5)
