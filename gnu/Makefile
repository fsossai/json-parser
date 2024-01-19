checker: compiled/parser.tab.c compiled/scanner.yy.c
	gcc -O3 -o $@ $^

compiled/parser.tab.c: parser.y
	bison -d --report=all -o $@ $<

compiled/scanner.yy.c: compiled/parser.tab.c
	flex -o $@ scanner.l

clean:
	rm -f checker
	rm -f compiled/parser.output
	rm -f compiled/parser.tab.c
	rm -f compiled/parser.tab.h
	rm -f compiled/scanner.yy.c
