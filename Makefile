bibly: bibly.c common.h string.h passage.h token.h lexer.h parser.h cite.h range.h binfo.h balt.h
	gcc bibly.c -o bibly -O3 -g -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -pedantic

.PHONY: clean run

run:
	gdb --args ./bibly kjv.vpl

clean:
	rm bibly


