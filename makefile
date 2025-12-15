CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I./headers
DEBUG_FLAGS = -g -fsanitize=address,undefined
RELEASE_FLAGS = -O2 -DNDEBUG

# Directories
SRC_DIR = ./src
BIN_DIR = ./bin
HEADER_DIR = ./headers

# Source files
SOURCES = $(SRC_DIR)/cdyar_darray.c
OBJECTS = $(BIN_DIR)/cdyar_darray.o

# Output library (static)
LIB_NAME = libcdyar.a
LIB_PATH = $(BIN_DIR)/$(LIB_NAME)

# Default target
all: $(LIB_PATH)

# Build static library
$(LIB_PATH): $(OBJECTS)
	ar rcs $@ $^

# Compile source files
$(BIN_DIR)/cdyar_darray.o: $(SRC_DIR)/cdyar_darray.c $(HEADER_DIR)/cdyar_darray.h $(HEADER_DIR)/cdyar_structures.h $(HEADER_DIR)/cdyar_error.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

# Clean build artifacts
clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/*.a

# Clean everything including directory
distclean: clean
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all debug release clean distclean
