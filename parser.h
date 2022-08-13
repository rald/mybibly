#ifndef PARSER_H
#define PARSER_H



#include "common.h"

#define PASSAGE_IMPLEMENTATION
#include "passage.h"

#define RANGE_IMPLEMENTATION
#include "range.h"

#define CITE_IMPLEMENTATION
#include "cite.h"



typedef enum ParserState {
  PARSERSTATE_DEFAULT=0,
  PARSERSTATE_BOOKNUMBER,
  PARSERSTATE_BOOK,
  PARSERSTATE_CHAPTERORVERSE,
  PARSERSTATE_CHAPTER,
  PARSERSTATE_CHAPTERRANGE,
  PARSERSTATE_VERS,
  PARSERSTATE_VERSRANGE,
  PARSERSTATE_EOF,
  PARSERSTATE_MAX  
} ParserState;



void Parser_Parse(Token **toks,size_t ntoks,Cite ***cites,size_t *ncites);



#ifdef PARSER_IMPLEMENTATION


static void Parser_Error(int code,char *fmt,...);
static void Parser_Next();
static void Parser_Prev();
static void match(TokenType type);
static bool Parser_Valid();
static bool Parser_ValidRel(ssize_t x);
static Token *Parser_Look();
static Token *Parser_LookRel(ssize_t x);
static void Parser_Expected(int code,size_t n,...);


static Token **tokens=NULL;
static size_t ntokens=0;
static ssize_t pc=0;




static void Parser_Error(int code,char *fmt,...) {
  va_list args;

  va_start(args,fmt);
    vprintf(fmt,args);
  va_end(args);

  exit(code);
}



static void Parser_Next() {
  pc++;
}



static void Parser_Prev() {
  pc--;
}



static void match(TokenType type) {
  if(!Parser_Valid() || Parser_Look()->type!=type) {
    Parser_Expected(1,1,type);
  }
}



static bool Parser_Valid() {
  return pc>=0 && pc<(ssize_t)ntokens;  
}



static bool Parser_ValidRel(ssize_t x) {
  return pc+x>=0 && pc+x<(ssize_t)ntokens;  
}



static Token *Parser_Look() {
  if(pc>=0 && pc<(ssize_t)ntokens) {
    return tokens[pc]; 
  }
  Parser_Error(1,"Error: program counter out of range: %zd",pc);
  return NULL;
}



static Token *Parser_LookRel(ssize_t x) {
  if(pc+x>=0 && pc+x<(ssize_t)ntokens) {
    return tokens[pc+x]; 
  } else {
    Parser_Error(1,"Error: program counter out of range: %zd",pc+x);
  }
  return NULL;
}



static void Parser_Expected(int code,size_t n,...) {

  va_list a;

  va_start(a,n);

    printf("Error: Expected ");

    for(size_t i=0;i<n;i++) {
      if(i!=0) { 
        if(i==n-1) printf(" or "); 
        else printf(", "); 
      }
      printf("%s",TokenTypeNames[va_arg(a,TokenType)]);
    }
    printf("; found %s at line: %zu column: %zu\n\n",TokenTypeNames[Parser_Look()->type],Parser_Look()->ln,Parser_Look()->cl);

  va_end(a);

  Token_Print(Parser_Look());

  printf("\n");

  exit(code);
}



int parse(Cite ***cites,size_t *ncites) {

  bool quit=false;

  size_t bnum=0;
  char * bname=NULL;
  

  size_t bchap=0;
  size_t echap=0;
  size_t bvers=0;
  size_t evers=0;

  Range *range=Range_New(0,0);

  Range **rverses=NULL;
  size_t nrverses=0;

  Cite *cite=Cite_New(0,NULL,Range_New(0,0),NULL,0);

  ParserState state=PARSERSTATE_DEFAULT;

  printf("<parse>\n");

  while(!quit) {

    switch(state) {

      case PARSERSTATE_DEFAULT: 
        printf("default\n");
        if(Parser_Valid()) {
          switch(Parser_Look()->type) {
          
            case TOKENTYPE_INTEGER:
              Parser_Prev();
              state=PARSERSTATE_BOOKNUMBER;
            break;

            case TOKENTYPE_STRING:
              Parser_Prev();
              state=PARSERSTATE_BOOK;        
            break;

            case TOKENTYPE_SEMICOLON:
            case TOKENTYPE_NEWLINE:
            break;

            case TOKENTYPE_EOF:
              state=PARSERSTATE_EOF;
            break;

            default: 
              Parser_Expected(1,3,
                TOKENTYPE_INTEGER,
                TOKENTYPE_STRING
              );  
          }
        }
      break;
      
      case PARSERSTATE_BOOKNUMBER: 
        printf("bnum\n");
        match(TOKENTYPE_INTEGER);
        sscanf(Parser_Look()->text,"%zu",&cite->bnum);
        state=PARSERSTATE_BOOK;
      break;

      case PARSERSTATE_BOOK: 
        printf("bname\n");
        match(TOKENTYPE_STRING);
        cite->bname=strdup(Parser_Look()->text);
        state=PARSERSTATE_CHAPTER;      
      break;

      case PARSERSTATE_CHAPTERORVERSE:
        printf("chaporvers\n");
        if(Parser_ValidRel(-1) && Parser_LookRel(-1)->type==TOKENTYPE_COMMA) {
          if(Parser_ValidRel(1)) {
            switch(Parser_LookRel(1)->type) {
              case TOKENTYPE_COLON:
                state=PARSERSTATE_CHAPTER;
                break;
              case TOKENTYPE_COMMA:
              case TOKENTYPE_DASH:
                state=PARSERSTATE_VERS;
                break;
              case TOKENTYPE_SEMICOLON:
              case TOKENTYPE_NEWLINE:
                state=PARSERSTATE_DEFAULT;
                break;
              default: break;
            }
          }
        } else {
          if(Parser_ValidRel(1)) {
            switch(Parser_LookRel(1)->type) {
              case TOKENTYPE_COLON:
              case TOKENTYPE_DASH:
                state=PARSERSTATE_CHAPTER;
                break;
              case TOKENTYPE_COMMA:
              case TOKENTYPE_SEMICOLON:
              case TOKENTYPE_NEWLINE:
                state=PARSERSTATE_VERS;
                break;
              default: break;              
            }        
          }
        }
        break;

      case PARSERSTATE_CHAPTER: 
        printf("chap\n");
        match(TOKENTYPE_INTEGER);
        sscanf(Parser_Look()->text,"%zu",&cite->rchap->begin); 
        cite->rchap->end=cite->rchap->begin;
        Parser_Next();
        if(Parser_Valid()) {
          switch(Parser_Look()->type) {
            case TOKENTYPE_COMMA:
              state=PARSERSTATE_CHAPTERORVERSE;
            break; 
            case TOKENTYPE_COLON:
              state=PARSERSTATE_VERS;
            break; 
            case TOKENTYPE_DASH: 
              Parser_Next();
              match(TOKENTYPE_INTEGER);
              sscanf(Parser_Look()->text,"%zu",&cite->rchap->end);
              state=PARSERSTATE_CHAPTERRANGE;
            break;
            case TOKENTYPE_SEMICOLON: 
            case TOKENTYPE_NEWLINE: 
              Cite_Append(cites,ncites,cite);
              cite=Cite_New(bnum,bname,Range_New(0,0),NULL,0);
              state=PARSERSTATE_DEFAULT;
            break;
            case TOKENTYPE_EOF: 
              Cite_Append(cites,ncites,cite);
              state=PARSERSTATE_EOF;
            break;
            default:
              state=PARSERSTATE_DEFAULT;
          }
        }
      break;

      case PARSERSTATE_CHAPTERRANGE:
        printf("chaprange\n");
        Cite_Append(cites,ncites,cite);
        cite=Cite_New(bnum,bname,Range_New(0,0),NULL,0);
        if(Parser_Valid()) {
          switch(Parser_Look()->type) {
            case TOKENTYPE_SEMICOLON:
            case TOKENTYPE_NEWLINE:
              state=PARSERSTATE_DEFAULT;
            break;
            case TOKENTYPE_EOF:
              state=PARSERSTATE_EOF;
            break;
            default:
              state=PARSERSTATE_DEFAULT;
          }
        }

        
        state=PARSERSTATE_DEFAULT;
      break;

      case PARSERSTATE_VERS: 
        printf("vers\n");
        match(TOKENTYPE_INTEGER);
        sscanf(Parser_Look()->text,"%zu",&range->begin);
        range->end=range->begin;
        Parser_Next();
        if(Parser_Valid()) {
          switch(Parser_Look()->type) {
            case TOKENTYPE_DASH: 
              Parser_Next();
              match(TOKENTYPE_INTEGER);
              sscanf(Parser_Look()->text,"%zu",&range->end);              
              state=PARSERSTATE_VERSRANGE;
            break;
            case TOKENTYPE_COMMA:
              Range_Append(&cite->rverses,&cite->nrverses,range);
              range=Range_New(0,0);
              state=PARSERSTATE_CHAPTERORVERSE;
            break;
            case TOKENTYPE_SEMICOLON:
            case TOKENTYPE_NEWLINE:
              Range_Append(&cite->rverses,&cite->nrverses,range);
              Cite_Append(cites,ncites,cite);
              range=Range_New(0,0);
              cite=Cite_New(0,NULL,Range_New(0,0),NULL,0);
              state=PARSERSTATE_DEFAULT;
            break;
            case TOKENTYPE_EOF: 
              Range_Append(&cite->rverses,&cite->nrverses,range);
              Cite_Append(cites,ncites,cite);                 state=PARSERSTATE_EOF;
            break;
            default:
              Parser_Expected(1,3,
                TOKENTYPE_DASH,
                TOKENTYPE_COMMA,
                TOKENTYPE_SEMICOLON
              );
          }
      }
      break;

      case PARSERSTATE_VERSRANGE:
        printf("vers range\n");
        Range_Append(&cite->rverses,&cite->nrverses,range);
        if(Parser_Valid()) {
          switch(Parser_Look()->type) {
            case TOKENTYPE_COMMA: 
              range=Range_New(0,0);
              state=PARSERSTATE_VERS;
            break;
            case TOKENTYPE_SEMICOLON: 
            case TOKENTYPE_NEWLINE: 
              Cite_Append(cites,ncites,cite);                 cite=Cite_New(0,NULL,Range_New(0,0),NULL,0);            
              range=Range_New(0,0);
              state=PARSERSTATE_DEFAULT;
            break;
            case TOKENTYPE_EOF: 
              Cite_Append(cites,ncites,cite);
              state=PARSERSTATE_EOF;
            break;
            default: 
              state=PARSERSTATE_DEFAULT;
          }
        }  
      break;

      case PARSERSTATE_EOF:
        printf("quit\n");
        quit=true;
      break;

      default: break;
    }

    Parser_Next();
  }

  printf("</parse>\n");

  Cites_Print(*cites,*ncites);

  return *ncites;

}



void Parser_Parse(Token **toks,size_t ntoks,Cite ***cites,size_t *ncites) {
  
  tokens=toks;
  ntokens=ntoks;
  pc=0;

  parse(cites,ncites);
}


#endif /* PARSER_IMPLEMENTATION*/



#endif /* PARSER_H */


