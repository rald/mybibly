bibly: bibly.c common.h mylib.h vector.h passage.h cite.h range.h 
	gcc bibly.c -o bibly -O3 -g -Wall -pedantic

clean:
	rm bibly

.PHONY: clean

