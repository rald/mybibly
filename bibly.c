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



int main(int argc,char **argv) {

	FILE *fp;

	char line[STRING_MAX];

	Token **tokens=NULL;
	size_t ntokens=0;

	Passage **passages=NULL;
	size_t npassages=0;
	
	Passage *passage=NULL;

  char **toks=NULL;
  size_t ntoks=0;

  Cite **cites;
  size_t ncites=0;  


	if(argc<2) {
    printf("Syntax: %s filename.vpl ...\n",argv[0]);
		return 1;
	}



	srand(time(NULL));

	fp=fopen(argv[1],"rt");

	while(fgets(line,STRING_MAX-2,fp)) {

		char *p=strchr(line,'\n');
		if(p) *p='\0';
			
		if(String_Strtok(line,"|",3,&toks,&ntoks)==4) {
			passage=Passage_New(
				toks[0],
				atoi(toks[1]),
				atoi(toks[2]),
				toks[3]
			);

			Passage_Append(&passages,&npassages,passage);

			Token_Free(toks);
			toks=NULL;
		}

	}



  Lexer_Lex(

"1 John 3:3-4,5:6-7"

,&tokens,&ntokens);




  Tokens_Print(tokens,ntokens);

  Parser_Parse(tokens,ntokens,&cites,&ncites);  
  for(size_t i=0;i<ntokens;i++) {
    Token_Free(tokens[i]);
    tokens[i]=NULL;
  }
  


	return 0;
}


