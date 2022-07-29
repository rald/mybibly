#ifndef CITE_H
#define CITE_H

#include "common.h"

#include "vector.h"
#include "range.h"

#include "passage.h"



typedef struct Cite Cite;

struct Cite {
	char *bname;
	Range *chaps;
	Vector *verses;
	char *text;
};



#ifdef CITE_IMPLEMETATION



Cite *Cite_New(char *bname,Range *chaps,Vector *verses) {
	Cite *cite=NEW1(cite);
	if(cite) {
		cite->bname=strdup(bname);
		cite->chap=chaps;
		cite->verses=verses;
	}
	return cite;
}



void Cite_Free(void *cite) {

	free(((Cite*)cite)->book);
	((Cite*)cite)->book=NULL;

	Range_Free((Cite*)cite)->chaps);
	(Cite*)cite)->chaps=NULL;

	Vector_Free((Cite*)cite)->verses);
	(Cite*)cite)->verses=NULL;

	free(cite);
}


void Cite_Parse(char *str,Cite ***cites,int *ncites) {
	
}



#endif /* CITE_IMPLEMETATION */



#endif /* CITE_H */


