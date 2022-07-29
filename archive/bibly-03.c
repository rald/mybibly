#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>



#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define VECTOR_IMPLEMENTATION
#include "vector.h"

#define PASSAGE_IMPLEMENTATION
#include "passage.h"

#define CITE_IMPLEMENTATION
#include "cite.h"



#define LINE_MAX 1024



int main(int argc,char **argv) {

	FILE *fp;

	char line[LINE_MAX];

	char **toks=NULL;
	int ntoks=0;

	if(argc<2) {
		return 1;
	}

	srand(time(NULL));

	Vector *passages=Vector_New(0,Passage_Print,Passage_Free);

	Passage *passage=NULL;

	fp=fopen(argv[1],"rt");

	while(fgets(line,LINE_MAX,fp)) {

		char *p=strchr(line,'\n');
		if(p) *p='\0';
			
		if(mystrtok(line,"|",3,&toks,&ntoks)==4) {	

			passage=Passage_New(
				toks[0],
				atoi(toks[1]),
				atoi(toks[2]),
				toks[3]
			);

			Vector_Append(passages,passage);
			mytokfree(&toks,&ntoks);
		}

	}


//	Passage_Print(0,Passage_SearchVerse(passages,"Romans",10,13));


//*
	Vector *searchPassages=Passage_SearchText(passages,argv[2]);

	Vector_Print(0,searchPassages);

	printf("Found %zu Occurrences\n\n",Vector_Length(searchPassages));

	Passage_SearchRandom(passages);
	
	Vector_Free(passages);


//*/


	return 0;
}


