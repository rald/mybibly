#ifndef CITE_H
#define CITE_H

#include "common.h"

#include "vector.h"
#include "range.h"

#include "passage.h"



typedef struct Cite Cite;

struct Cite {
	char *book;
	Range *chaps;
	Vector *verses;
	char *text;
};




#ifdef CITE_IMPLEMETATION



Cite *Cite_New() {
	Cite *cite=NEW1(cite);
	if(cite) {
		cite->book=book;
		cite->chap=chaps;
		cite->verses=Vector_Create(0,Cite_Print,Cite_Free);
	}
	return cite;
}
	

void Cite_Free(Cite **cite) {
	free((*cite)->book);
	(*cite)->book=NULL;

	Range_Free((*cite)->chaps);

	Vector_Free((*cite)->verses);
}


#endif /* CITE_IMPLEMETATION */

#endif /* CITE */
