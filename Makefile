SRC=src/*.cpp
INC_DIR=inc
FLAGS=-Wall -Wextra -O3
CC=g++
SHELL=/bin/bash
CMD=checker
TARGETS=checker prettify stringify

.PHONY: test benchmark clean

all: $(TARGETS)

$(TARGETS): % : %.cpp
	$(CC) $(FLAGS) -o $@ -Iinc src/*.cpp $<

gnuchecker: gnu/compiled/parser.tab.c gnu/compiled/scanner.yy.c
	gcc -lfl -O3 -o $@ $^

gnu/compiled/parser.tab.c: gnu/parser.y
	bison -d --report=all -o $@ $<

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
	rm -f $(TARGETS)
