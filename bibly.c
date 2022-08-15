#define _GNU_SOURCE



#define COMMON_IMPLEMENTATION
#include "common.h"

#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define PASSAGE_IMPLEMENTATION
#include "passage.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define LEXER_IMPLEMENTATION
#include "lexer.h"

#define PARSER_IMPLEMENTATION
#include "parser.h"

#define BINFO_IMPLEMENTATION
#include "binfo.h"

#define BALT_IMPLEMENTATION
#include "balt.h"



int main(int argc,char **argv) {

	FILE *fin=NULL;

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;

	Token **tokens=NULL;
	size_t ntokens=0;

	Passage **passages=NULL;
	size_t npassages=0;
	
	Passage *passage=NULL;

	Passage **fpassages=NULL;
	size_t nfpassages=0;

  char **toks=NULL;
  size_t ntoks=0;

  Cite **cites;
  size_t ncites=0;  



	if(argc<2) {
    printf("Syntax: %s filename.vpl ...\n",argv[0]);
		return 1;
	}


	
	srand(time(NULL));

	fin=fopen(argv[1],"rt");

  while((rlen=getline(&line,&llen,fin))>0) {

    String_Strrnl(line);
			
    if(String_Strtok(line,"|",3,&toks,&ntoks)==4) {
			passage=Passage_New(
				toks[0],
				atoi(toks[1]),
				atoi(toks[2]),
				toks[3]
			);

			Passage_Append(&passages,&npassages,passage);
	
			Strings_Free(&toks,&ntoks);
			toks=NULL;
			ntoks=0;
			
    }

    free(line);
    line=NULL;
    llen=0;
    rlen=0;

	}



Lexer_Lex(
"Jn 3:16,20,22,24-27"
,&tokens,&ntokens);



  Parser_Parse(tokens,ntokens,&cites,&ncites);  

  Cites_Print(cites,ncites);

  Cites_ToPassages(cites,ncites,passages,npassages,&fpassages,&nfpassages);

  Passages_Print(fpassages,nfpassages);

  Passages_Free(&fpassages,&nfpassages);
  Passages_Free(&passages,&npassages);

  
	return 0;
}


