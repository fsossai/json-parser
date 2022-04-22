SRC=src/*.cpp
INC_DIR=inc
FLAGS=-Wall -Wextra -O3
CC=g++
SHELL=/bin/bash
CMD=checker

.PHONY: test benchmark clean

all: checker prettify

checker:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) checker.cpp

prettify:
	$(CC) $(FLAGS) -o $@ -I$(INC_DIR) $(SRC) prettify.cpp

gnuchecker: gnu/compiled/parser.tab.c gnu/compiled/scanner.yy.c
	gcc -lfl -O3 -o $@ $^

gnu/compiled/parser.tab.c:
	bison -d --report=all -o $@ gnu/parser.y

gnu/compiled/scanner.yy.c: gnu/compiled/parser.tab.c
	flex -o $@ gnu/scanner.l


# Phony targets

test: $(CMD)
	$(SHELL) test.sh $(CMD) > /dev/null

benchmark: $(CMD)
	$(SHELL) benchmark.sh ./$(CMD) | column -s, -t

clean:
	rm -f gnu/compiled/*.output
	rm -f gnu/compiled/*.c
	rm -f gnu/compiled/*.h
	rm -f checker gnuchecker prettify
