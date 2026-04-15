# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -g

# Windows (MinGW/MSYS): link Winsock for transport_socket.c
ifeq ($(OS),Windows_NT)
LDFLAGS = -lws2_32
endif

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .
AES_SRC = lib/aes.c
AES_OBJ = $(OBJ_DIR)/aes_lib.o

# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate object file names from source files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Executable name
TARGET = $(BIN_DIR)/file_transfer.exe

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS) $(AES_OBJ)
	$(CC) $(OBJS) $(AES_OBJ) -o $@ $(LDFLAGS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# tiny-AES-c lives under lib/ (not under src/)
$(AES_OBJ): $(AES_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
