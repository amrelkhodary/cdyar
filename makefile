CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I./headers
DEBUG_FLAGS = -g -fsanitize=address,undefined
RELEASE_FLAGS = -O2 -DNDEBUG
# Directories
SRC_DIR = ./src
BIN_DIR = ./bin
HEADER_DIR = ./headers
INSTALL_PREFIX = /usr/local
INCLUDE_LIBDIR = ${INSTALL_PREFIX}/include
LIBDIR = ${INSTALL_PREFIX}/lib

# Source files
SOURCES = $(SRC_DIR)/cdyar_darray.c $(SRC_DIR)/cdyar_types.c $(SRC_DIR)/cdyar_arithmetic.c $(SRC_DIR)/cdyar_error.c
OBJECTS = $(BIN_DIR)/cdyar_darray.o $(BIN_DIR)/cdyar_types.o $(BIN_DIR)/cdyar_arithmetic.o $(BIN_DIR)/cdyar_error.o
# Output library (static)
LIB_NAME = libcdyar.a
LIB_PATH = $(BIN_DIR)/$(LIB_NAME)
# Executable
EXEC_NAME = cdyar
EXEC_PATH = $(BIN_DIR)/$(EXEC_NAME)
MAIN_OBJ = $(BIN_DIR)/main.o
# Default target
all: $(LIB_PATH) $(EXEC_PATH)
# Build static library
$(LIB_PATH): $(OBJECTS)
	ar rcs $@ $^
# Build executable
$(EXEC_PATH): $(MAIN_OBJ) $(LIB_PATH)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $< -L$(BIN_DIR) -lcdyar -o $@
# Compile source files
$(BIN_DIR)/cdyar_darray.o: $(SRC_DIR)/cdyar_darray.c $(HEADER_DIR)/cdyar_darray.h $(HEADER_DIR)/cdyar_structures.h $(HEADER_DIR)/cdyar_error.h $(HEADER_DIR)/cdyar_types.h $(HEADER_DIR)/cdyar_macros.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@
$(BIN_DIR)/cdyar_types.o: $(SRC_DIR)/cdyar_types.c $(HEADER_DIR)/cdyar_types.h $(HEADER_DIR)/cdyar_error.h $(HEADER_DIR)/cdyar_macros.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@
$(BIN_DIR)/cdyar_arithmetic.o: $(SRC_DIR)/cdyar_arithmetic.c $(HEADER_DIR)/cdyar_arithmetic.h $(HEADER_DIR)/cdyar_types.h $(HEADER_DIR)/cdyar_error.h $(HEADER_DIR)/cdyar_macros.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@
$(BIN_DIR)/cdyar_error.o: $(SRC_DIR)/cdyar_error.c $(HEADER_DIR)/cdyar_error.h $(HEADER_DIR)/cdyar_macros.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@
# Compile main.c
$(MAIN_OBJ): $(SRC_DIR)/main.c $(HEADER_DIR)/cdyar.h $(HEADER_DIR)/cdyar_darray.h $(HEADER_DIR)/cdyar_structures.h $(HEADER_DIR)/cdyar_error.h $(HEADER_DIR)/cdyar_types.h $(HEADER_DIR)/cdyar_arithmetic.h $(HEADER_DIR)/cdyar_macros.h | $(BIN_DIR)
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

install:
	install -D -m 644 cdyar.h $(INCLUDEDIR)/cdyar.h
	install -D -m 644 libcdyar.a $(LIBDIR)/libcdyar.a

uninstall:
	rm -f $(INCLUDEDIR)/cdyar.h
	rm -f $(LIBDIR)/libcdyar.a
# Clean build artifacts
clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/*.a $(BIN_DIR)/$(EXEC_NAME)
# Clean everything including directory
distclean: clean
	rm -rf $(BIN_DIR)
# Phony targets
.PHONY: all debug release clean distclean
