# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -std=c11

# Target Executable
TARGET = dungeon

# Source Files
SRCS = dungeon.c

# Header Files
HEADERS = game.h

# Object Files (derived from source files)
OBJS = $(SRCS:.c=.o)

# Default Rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files from source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not associated with files)
.PHONY: all clean

