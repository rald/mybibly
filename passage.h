#ifndef PASSAGE_H
#define PASSAGE_H



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



#define VECTOR_IMPLEMENtATION
#include "vector.h"



typedef struct Passage Passage;

struct Passage {
	char *bname;
	int cnum;
	int vnum;
	char *text;
};



Passage *Passage_New(char *book,int cnum,int vnum,char *text);
void Passage_Free(void *passage);
void Passage_Print(int num,void *passage);
Passage *Passage_SearchVerse(Vector *passages,char *bname,int cnum,int vnum);
Vector *Passage_SearchText(Vector *passages,char *pattern);
void Passage_SearchRandom(Vector *passages);


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



void Passage_Print(int num,void *passage) {

	Passage *p=(Passage*)passage;

	if(num==0) {

		printf("%s %d:%d -> %s\n\n",
			p->bname,
			p->cnum,
			p->vnum,
			p->text
		);	

	} else {
	
		printf("(%d) %s %d:%d -> %s\n\n",
			num,
			p->bname,
			p->cnum,
			p->vnum,
			p->text
		);	
		
	}
}



Passage *Passage_SearchVerse(Vector *passages,char *bname,int cnum,int vnum) {

	for(int i=0;i<Vector_Length(passages);i++) {

		Passage *passage=Vector_Get(passages,i);
	
		if(!strcasecmp(bname,passage->bname) && cnum==passage->cnum && vnum==passage->vnum) {	

			return passage;		
		}
	}

	return NULL;
}



Vector *Passage_SearchText(Vector *passages,char *pattern) {

	Vector *foundPassages=Vector_New(0,Passage_Print,Passage_Free);

	for(int i=0;i<Vector_Length(passages);i++) {

		Passage *passage=Vector_Get(passages,i);
	
		if(strcasestr(passage->text,pattern) || strmatch(passage->text,pattern,strlen(passage->text),strlen(pattern))) {	

			Vector_Append(foundPassages,passage);		
		}
	}

	return foundPassages;
}



void Passage_SearchRandom(Vector *passages) {
	Passage_Print(0,Vector_Get(passages,rand()%Vector_Length(passages)));
}


#endif /* PASSAGE_IMPLEMENTATION */



#endif /* PASSAGE_H */


