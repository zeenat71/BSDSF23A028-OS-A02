# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -g

# Source and target
SRC = src/ls-v1.0.0.c
OBJ = obj/ls-v1.0.0.o
TARGET = bin/ls

# Build target
all: $(TARGET)

# Compile
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(OBJ)
