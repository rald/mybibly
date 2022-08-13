bibly: bibly.c common.h string.h passage.h token.h lexer.h parser.h cite.h range.h 
	gcc bibly.c -o bibly -O3 -g -Wall -Wextra -Wno-unused-variable -pedantic

clean:
	rm bibly

.PHONY: clean

