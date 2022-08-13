#ifndef TOKEN_H
#define TOKEN_H



#define COMMON_IMPLEMENTATION
#include "common.h"



typedef enum TokenType {
	TOKENTYPE_NULL=0,  
	TOKENTYPE_INTEGER,
	TOKENTYPE_STRING,
	TOKENTYPE_COLON,
	TOKENTYPE_DASH,
	TOKENTYPE_COMMA,	
	TOKENTYPE_SEMICOLON,	
	TOKENTYPE_NEWLINE,	
	TOKENTYPE_EOF,
	TOKENTYPE_MAX
} TokenType;



typedef struct Token Token;

struct Token {
	TokenType type;
	char *text;
	size_t ln,cl;
};



Token *Token_New(TokenType type,char *text,size_t ln,size_t cl);

void Token_Free(void *token);

void Token_Append(Token ***tokens,size_t *ntokens,Token *token);

void Token_Print(void *token);



#ifdef TOKEN_IMPLEMENTATION



char *TokenTypeNames[] = {
  "null",
	"integer",
	"string",
	"colon",
	"dash",
	"comma",
	"semicolon",
	"newline",
	"eof"
};



Token *Token_New(TokenType type,char *text,size_t ln,size_t cl) {
	Token *token=malloc(sizeof(*token));
	if(token) {
		token->type=type;
		token->text=text?strdup(text):NULL;
		token->ln=ln;
		token->cl=cl;
	}
	return token;
}



void Token_Free(void *token) {
  free(((Token*)token)->text);
  ((Token*)token)->text=NULL;
  
	free(token);
}



void Token_Append(Token ***tokens,size_t *ntokens,Token *token) {
  (*tokens)=realloc(*tokens,sizeof(**tokens)*((*ntokens)+1));
  (*tokens)[(*ntokens)++]=token;
}



void Token_Print(void *token) {
	printf("{ \"token\": { \"type\": \"%s\", \"text\": \"%s\", line: %zu, column: %zu } }\n",
  	TokenTypeNames[((Token*)token)->type],
  	((Token*)token)->text,
  	((Token*)token)->ln,
  	((Token*)token)->cl
  );
}



void Tokens_Print(Token **tokens,size_t ntokens) {
  for(size_t i=0;i<ntokens;i++) {
    Token_Print(tokens[i]);
  }
  printf("\n");
}




#endif /* TOKEN_IMPLEMENTATION */



#endif /* TOKEN_H */


