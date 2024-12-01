
# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Source files and output
SRC = game.c
HEADERS = game.h
OBJ = $(SRC:.c=.o)
OUTPUT = game

# Default target
all: $(OUTPUT)

# Build the executable
$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJ) $(OUTPUT)

# Run the game
run: $(OUTPUT)
	./$(OUTPUT)
