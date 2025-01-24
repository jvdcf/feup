# Makefile for FTP Client
# Authors: Guilherme Matos, João Ferreira

# Compiler
CC = gcc
# Flags for better error checking and warnings	
CFLAGS = -Wall -Wextra -fsanitize=address
# Optimization flag
CFLAGS += -O2
# C standard: Computer Labs use gcc 8.3.0-6
CFLAGS += -std=c17

# Source files (.c)
SRC = src
# Header files (.h)
INCLUDE = include
# Binaries
BIN = bin
# Executable name
NAME = download

.PHONY: all
all: clean compile

.PHONY: run
run:
	$(BIN)/$(NAME)

.PHONY: test
test:
	$(BIN)/$(NAME) ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt

.PHONY: compile
compile: main.c $(SRC)/*.c
	$(CC) $(CFLAGS) -o $(BIN)/$(NAME) $^ -I$(INCLUDE)

.PHONY: clean
clean:
	rm -f $(BIN)/$(NAME)
