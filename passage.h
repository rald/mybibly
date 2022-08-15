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

void Passage_Free(Passage **passage);

void Passages_Free(Passage ***passages,size_t *npassages);

void Passage_Append(Passage ***passages,size_t *npassages,Passage *passage);

void Passage_Print(Passage *passage);

void Passages_Print(Passage **passages,size_t npassages);

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



void Passage_Free(Passage **passage) {
  if(passage && *passage) {
  	free((*passage)->bname);
  	(*passage)->bname=NULL;
  	free((*passage)->text);
  	(*passage)->text=NULL;
  	free(*passage);
  	(*passage)=NULL;
	}
}



void Passages_Free(Passage ***passages,size_t *npassages) {

  if(passages && *passages) {

    for(size_t i=0;i<(*npassages);i++) {
      Passage_Free(&(*passages)[i]);
    }
    free(passages);
    passages=NULL;
    npassages=0;

  }
}



void Passage_Append(Passage ***passages,size_t *npassages,Passage *passage) {
  (*passages)=realloc(*passages,sizeof(*passages)*((*npassages)+1));
  (*passages)[(*npassages)++]=passage;
}



void Passage_Print(Passage *passage) {
  if(passage) {
    printf("%s %zu:%zu -> %s\n\n",
    	passage->bname,
    	passage->cnum,
    	passage->vnum,
    	passage->text
    );			
  } else {
    printf("(null)\n\n");
  }
}



void Passages_Print(Passage **passages,size_t npassages) {
  printf("npassages: %zu\n",npassages);
  for(size_t i=0;i<npassages;i++) {
    Passage_Print(passages[i]);
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


