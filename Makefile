SRC=src/*.cpp
INC_DIR=inc
FLAGS=-Wall -Wextra -O3
CC=g++

.PHONY: formatter

all: formatter

formatter:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) formatter.cpp
