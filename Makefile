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

gnuchecker: gnu/compiled/parser.tab.c gnu/compiled/scanner.yy.c
	gcc -lfl -Ofast -o $@ $^

gnu/compiled/parser.tab.c:
	bison -d --report=all -o $@ gnu/parser.y

gnu/compiled/scanner.yy.c: gnu/compiled/parser.tab.c
	flex -o $@ gnu/scanner.l
