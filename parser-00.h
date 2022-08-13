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
  PARSERSTATE_INTEGER,
  PARSERSTATE_BOOK,
  PARSERSTATE_CHAPTER,
  PARSERSTATE_CHAPTERRANGE,
  PARSERSTATE_VERS,
  PARSERSTATE_VERSRANGE,
  PARSERSTATE_MAX  
} ParserState;




void Parser_Parse(Token **toks,size_t ntoks,Cite ***cites,size_t *ncites);




#ifdef PARSER_IMPLEMENTATION



static void next();
static void expected(int code,size_t n,...);
static Token *look();


static Token **tokens=NULL;
static size_t ntokens=0;
static size_t pc=0;





static void next() {
  pc++;
}

static void prev() {
  pc--;
}

static Token *look() {
  if(pc>=0 && pc<ntokens) {
    return tokens[pc];
  }
  return NULL;
}


static void expected(int code,size_t n,...) {

  va_list a;

  va_start(a,n);

    printf("Error: Expected ");

    for(size_t i=0;i<n;i++) {
      if(i!=0) printf(", ");
      printf("%s",TokenTypeNames[va_arg(a,TokenType)]);
    }
    printf(" found %s at line: %zu column: %zu\n\n",TokenTypeNames[look()->type],look()->ln,look()->cl);

  va_end(a);

  Token_Print(look());

  printf("\n");

  exit(code);
}




int parse(Cite ***cites,size_t *ncites) {

  char book[STRING_MAX]="";
  size_t bchap=0;
  size_t echap=0;
  size_t bvers=0;
  size_t evers=0;

  Range **rverses=NULL;
  size_t nrverses=0;

  ParserState state=PARSERSTATE_DEFAULT;

  printf("<parse>\n");

  for(;;) {

    Token_Print(look());

    switch(state) {

      case PARSERSTATE_DEFAULT: 
        printf("default\n");
        if(look() && look()->type==TOKENTYPE_INTEGER) {
          book[0]='\0';
          state=PARSERSTATE_INTEGER;
          prev();
        } else if(look() && look()->type==TOKENTYPE_STRING) {
          book[0]='\0';
          state=PARSERSTATE_BOOK;        
          prev();
        }
      break;
      
      case PARSERSTATE_INTEGER: 
        printf("integer\n");
        if(look() && look()->type==TOKENTYPE_INTEGER) {
          strcpy(book,look()->text);
          state=PARSERSTATE_BOOK;
        } else {
          expected(1,1,TOKENTYPE_INTEGER);
        }
      break;

      case PARSERSTATE_BOOK: 
        printf("book\n");
        if(look() && look()->type==TOKENTYPE_STRING) {
          if(*book) strcat(book," ");
          strcat(book,look()->text);
          state=PARSERSTATE_CHAPTER;
        } else {
          expected(1,1,TOKENTYPE_STRING);
        }
      break;

      case PARSERSTATE_CHAPTER: 
        printf("chapter\n");
        if(look() && look()->type==TOKENTYPE_INTEGER) {
          bchap=atoi(look()->text);
          echap=0;
          next();
          if(look() && look()->type==TOKENTYPE_DASH) {
            next();
            if(look() && look()->type==TOKENTYPE_INTEGER) {
              echap=atoi(look()->text);
              state=PARSERSTATE_CHAPTERRANGE;
            } else {                    
              expected(1,1,TOKENTYPE_INTEGER);
            }  
          } else if(look() && look()->type==TOKENTYPE_COLON) {  
            state=PARSERSTATE_VERS;
          } else if(look() && look()->type==TOKENTYPE_SEMICOLON) {
            Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),NULL,0));
            state=PARSERSTATE_DEFAULT;
          } else if(look() && look()->type==TOKENTYPE_EOF) {
            Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),NULL,0));
            state=PARSERSTATE_DEFAULT;
          } else {
            expected(1,3,
              TOKENTYPE_DASH,
              TOKENTYPE_COLON,
              TOKENTYPE_SEMICOLON
            );
          }
        } else {
          expected(1,1,TOKENTYPE_INTEGER);
        }
      break;

      case PARSERSTATE_CHAPTERRANGE:
        printf("chapter range\n");
        Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),NULL,0));

        if(look() && look()->type==TOKENTYPE_SEMICOLON) {
          state=PARSERSTATE_DEFAULT;
        } else {                    
          expected(1,1,TOKENTYPE_SEMICOLON);
        }  
      break;

      case PARSERSTATE_VERS: 
        printf("vers\n");
        if(look() && look()->type==TOKENTYPE_INTEGER) {
          bvers=atoi(look()->text);
          evers=0;
          next();
          if(look() && look()->type==TOKENTYPE_DASH) {
            next();
            if(look() && look()->type==TOKENTYPE_INTEGER) {
              evers=atoi(look()->text);
              state=PARSERSTATE_VERSRANGE;
            } else {                    
              expected(1,1,TOKENTYPE_INTEGER);
            }  
          } if(look() && look()->type==TOKENTYPE_COMMA) {  
            Range_Append(&rverses,&nrverses,Range_New(bvers,evers));          
            state=PARSERSTATE_VERS;
          } if(look() && look()->type==TOKENTYPE_SEMICOLON) {
            Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),rverses,nrverses));
            rverses=malloc(sizeof(*rverses));
            nrverses=0;
            state=PARSERSTATE_DEFAULT;
          } if(look() && look()->type==TOKENTYPE_EOF) {
            Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),rverses,nrverses));
            rverses=NULL;
            nrverses=0;
            state=PARSERSTATE_DEFAULT;
          } else {
            expected(1,3,
              TOKENTYPE_DASH,
              TOKENTYPE_COLON,
              TOKENTYPE_SEMICOLON
            );
          }
        } else {
          expected(1,1,TOKENTYPE_INTEGER);
        }
      break;

      case PARSERSTATE_VERSRANGE:
        printf("vers range\n");
        Range_Append(&rverses,&nrverses,Range_New(bvers,evers));
         
        if(look() && look()->type==TOKENTYPE_COMMA) {
          state=PARSERSTATE_VERS;          
        } else if(look() && look()->type==TOKENTYPE_SEMICOLON) {
          Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),rverses,nrverses));       
          rverses=malloc(sizeof(*rverses));
          nrverses=0;

          state=PARSERSTATE_DEFAULT;
        } else if(look() && look()->type==TOKENTYPE_EOF) {
          Cite_Append(cites,ncites,Cite_New(book,Range_New(bchap,echap),rverses,nrverses));    

          rverses=malloc(sizeof(*rverses));
          nrverses=0;
                      
          state=PARSERSTATE_DEFAULT;
        } else {                    
          expected(1,2,
            TOKENTYPE_COMMA,
            TOKENTYPE_SEMICOLON
          );
        }  

      break;

      default: break;
    }

    if(look() || look()->type==TOKENTYPE_EOF) break;

    next();

  }

  printf("</parse>\n");

  return *ncites;

}



void Parser_Parse(Token **toks,size_t ntoks,Cite ***cites,size_t *ncites) {
  
  tokens=toks;
  ntokens=ntoks;
  pc=0;

  parse(cites,ncites);
}

#endif

#endif /* PARSER_H */
