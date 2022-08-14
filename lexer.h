#ifndef LEXER_H
#define LEXER_H



#define COMMON_IMPLEMENTATION
#include "common.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define STRING_IMPLEMENTATION
#include "string.h"



typedef enum LexerState {
	LEXERSTATE_DEFAULT=0,
	LEXERSTATE_INTEGER,
	LEXERSTATE_STRING,	
	LEXERSTATE_EOF,
	LEXERSTATE_MAX		
} LexerState;



size_t Lexer_Lex(char *line,Token ***tokens,size_t *ntokens);




#ifdef LEXER_IMPLEMENTATION

static char *cites=NULL;

static size_t ln=0,cl=0;
static size_t sln=0,scl=0;

static size_t lpc=0;



static void Lexer_Error(int code,char *fmt,...);

static void Lexer_Next();
static void Lexer_Prev();



static void Lexer_Error(int code,char *fmt,...) {

  va_list args;

  va_start(args,fmt);
    vprintf(fmt,args);
  va_end(args);

  exit(code);
}



static void Lexer_Next() {
  lpc++; cl++;
}



static void Lexer_Prev() {
  lpc--; 
  cl--;
  if(cites[lpc]=='\n') {
    int j=lpc-1;
    ln--;
    cl=0;
    while(j>=0 && cites[j]!='\n') { 
      cl++; 
      j--; 
    }
  } 
}



size_t Lexer_Lex(char *line,Token ***tokens,size_t *ntokens) {

  bool quit=false;

	LexerState state=LEXERSTATE_DEFAULT;

	char *text=NULL;

	cites=line;
	
  while(!quit) {

		char c=cites[lpc];

		char s[2]={c,'\0'};

		switch(state) {

			case LEXERSTATE_DEFAULT:

        if(c=='\0') {
          state=LEXERSTATE_EOF;
				} else if(isdigit(c)) {
				  sln=ln;
				  scl=cl;
					lpc--;
          cl--;
					state=LEXERSTATE_INTEGER;
				} else if(isalpha(c)) {
				  sln=ln;
				  scl=cl;
					lpc--;
					cl--;
					state=LEXERSTATE_STRING;
				} else if(String_IndexOfChar(":-,;",c)!=-1) {
				  TokenType type=TOKENTYPE_NULL;
          switch(c) {
            case ':': type=TOKENTYPE_COLON; break;
            case '-': type=TOKENTYPE_DASH; break;
            case ',': type=TOKENTYPE_COMMA;  break;    
            case ';': type=TOKENTYPE_SEMICOLON; break;    
          }
          Token_Append(tokens,ntokens,Token_New(type,s,ln,cl));						
        } else if(c=='\n') {
          Token_Append(tokens,ntokens,Token_New(TOKENTYPE_NEWLINE,s,ln,cl));						
          ln++;
          cl=0;
			  } else if(isspace(c)) {
				  while(isspace(cites[lpc])) {
				    Lexer_Next();
				  }
				  Lexer_Prev();
        } else {
          Lexer_Error(1,"Error: invalid character '0x%02X' at line %zu column %zu",c,ln,cl);
        }

			break;

			case LEXERSTATE_INTEGER:
				if(isdigit(c)) {
          String_Append(&text,"%c",c);
        } else {
					Token_Append(tokens,ntokens,Token_New(TOKENTYPE_INTEGER,text,sln,scl));
          free(text);
          text=NULL;
					state=LEXERSTATE_DEFAULT;
					Lexer_Prev();					
				}
			break;

			case LEXERSTATE_STRING:
				if(isalpha(c) || isspace(c)) {
          String_Append(&text,"%c",c);
				} else {
					Token_Append(tokens,ntokens,Token_New(TOKENTYPE_STRING,String_Trim(text),sln,scl));	
          free(text);
          text=NULL;
					state=LEXERSTATE_DEFAULT;
					Lexer_Prev();
				}
			break;

			case LEXERSTATE_EOF:
			  quit=true;
			break;
			

			default: break; 
		}
 
	  Lexer_Next();
	}

	Token_Append(tokens,ntokens,Token_New(TOKENTYPE_EOF,NULL,ln,cl));	

	return *ntokens;
}



#endif /* LEXER_IMPLEMENTATION */



#endif /* LEXER_H */



