FLAGS=-Wall -Wextra -O3
CC=g++
CMD=checker
TOOLS=checker prettify stringify
export

.PHONY: test benchmark custom clean

all: $(TOOLS)

$(TOOLS): % : %.cpp lib/libjp.a
	$(CC) $(FLAGS) -o $@ -Iinc $< -ljp -Llib

lib/libjp.a:
	$(MAKE)	-C lib

gnuchecker: gnu/compiled/parser.tab.c gnu/compiled/scanner.yy.c
	gcc -lfl -O3 -o $@ $^

gnu/compiled/parser.tab.c: gnu/parser.y
	bison -d --report=all -o $@ $<

gnu/compiled/scanner.yy.c: gnu/compiled/parser.tab.c
	flex -o $@ gnu/scanner.l

# Phony targets

test: $(CMD)
	/bin/bash test.sh $(CMD) > /dev/null

benchmark: $(CMD)
	/bin/bash benchmark.sh ./$(CMD) | column -s, -t

clean:
	rm -f $(TOOLS) gnuchecker
	rm -f lib/*.o
	rm -f lib/*.a
	rm -f gnu/compiled/*.output
	rm -f gnu/compiled/*.c
	rm -f gnu/compiled/*.h
