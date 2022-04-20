SRC=src/*.cpp
INC_DIR=inc
FLAGS=-Wall -Wextra -O3
CC=g++

.PHONY: checker formatter

all: checker formatter

checker:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) checker.cpp

formatter:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) formatter.cpp
