#ifndef PASSAGE_H
#define PASSAGE_H



#define COMMON_IMPLEMENTATION
#include "common.h"



typedef struct Passage Passage;

struct Passage {
	char *bname;
	size_t cnum;
	size_t vnum;
	char *text;
};



Passage *Passage_New(char *book,int cnum,int vnum,char *text);

void Passage_Free(void *passage);

void Passage_Append(Passage ***passages,size_t *npassages,Passage *passage);

void Passage_Print(int num,void *passage);

Passage *Passage_SearchVerse(Passage **passages,size_t npassages,char *bname,size_t cnum,size_t vnum);

Passage *Passage_SearchRandom(Passage **passages,size_t npassages);

int Passage_SearchText(Passage **passages,size_t npassages,Passage ***fpassages,size_t *nfpassages,char *pattern);



#ifdef PASSAGE_IMPLEMENTATION



Passage *Passage_New(char *book,int cnum,int vnum,char *text) {
	Passage *passage=NEW1(passage);
	if(passage) {
		passage->bname=strdup(book);
		passage->cnum=cnum;
		passage->vnum=vnum;
		passage->text=strdup(text);
	}
	return passage;
}



void Passage_Free(void *passage) {
	free(((Passage*)passage)->bname);
	free(((Passage*)passage)->text);
	free(passage);
}



void Passage_Append(Passage ***passages,size_t *npassages,Passage *passage) {
  (*passages)=realloc(*passages,sizeof(*passages)*((*npassages)+1));
  (*passages)[(*npassages)++]=passage;
}



void Passage_Print(int num,void *passage) {

	Passage *p=(Passage*)passage;

	if(num==0) {

		printf("%s %zu:%zu -> %s\n\n",
			p->bname,
			p->cnum,
			p->vnum,
			p->text
		);	

	} else {
	
		printf("(%d) %s %zu:%zu -> %s\n\n",
			num,
			p->bname,
			p->cnum,
			p->vnum,
			p->text
		);	
		
	}
}



Passage *Passage_SearchVerse(Passage **passages,size_t npassages,char *bname,size_t cnum,size_t vnum) {

	for(size_t i=0;i<npassages;i++) {

		if(!strcasecmp(bname,passages[i]->bname) && cnum==passages[i]->cnum && vnum==passages[i]->vnum) {	

			return passages[i];		
		}
	}

	return NULL;
}



int Passage_SearchText(Passage **passages,size_t npassages,Passage ***fpassages,size_t *nfpassages,char *pattern) {

	for(size_t i=0;i<npassages;i++) {
		if(strcasestr(passages[i]->text,pattern) || strmatch(passages[i]->text,pattern,strlen(passages[i]->text),strlen(pattern))) {	
			Passage_Append(fpassages,nfpassages,passages[i]);		
		}
	}

	return *nfpassages;
}



Passage *Passage_SearchRandom(Passage **passages,size_t npassages) {
	return passages[rand()%npassages];
}



#endif /* PASSAGE_IMPLEMENTATION */



#endif /* PASSAGE_H */


