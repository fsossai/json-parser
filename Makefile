SRC=src/*.cpp
INC_DIR=inc
FLAGS=-Wall -Wextra -O3
CC=g++
SHELL=/bin/bash

.PHONY: test benchmark

all: checker formatter

checker:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) checker.cpp

formatter:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) formatter.cpp

test: checker
	$(SHELL) test.sh > /dev/null

benchmark: checker
	$(SHELL) benchmark.sh | column -s, -t
