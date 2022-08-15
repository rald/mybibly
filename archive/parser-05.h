#ifndef PARSER_H
#define PARSER_H



#include "common.h"

#define STRING_IMPLEMENTATION
#include "string.h"

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

static void Parser_Expected(int code,size_t n,...);

static bool Parser_Valid();
static bool Parser_ValidRel(ssize_t offset);

static void Parser_MatchExpect(TokenType type);
static bool Parser_Match(TokenType type);
static bool Parser_MatchRel(ssize_t offset,TokenType type);


static Token *Parser_Look();
static Token *Parser_LookRel(ssize_t offset);

static int Parser_GetInteger();
static char *Parser_GetString();

static void Parser_ParseVers(Cite ***cites,size_t *ncites);
static void Parser_ParseChapter(Cite ***cites,size_t *ncites);
static void Parser_ParseBook(Cite ***cites,size_t *ncites);

static void Parser_ParseStart(Cite ***cites,size_t *ncites);

void Parser_Parse(Token **toks,size_t ntoks,Cite ***cites,size_t *ncites);



static Token **tokens=NULL;
static size_t ntokens=0;
static ssize_t pc=0;



static size_t bnum=0;
static char *bname=NULL;
static size_t bcnum=0;
static size_t ecnum=0;
static size_t bvnum=0;
static size_t evnum=0;

static Range *crange=NULL;
static Range *vrange=NULL;
static Range **vranges=NULL;
static size_t nvranges=0;

static Cite *cite=NULL;



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



static void Parser_MatchExpect(TokenType type) {
  if(!Parser_Valid() || Parser_Look()->type!=type) {
    Parser_Expected(1,1,type);
  }
}


static bool Parser_Valid() {
  return pc>=0 && pc<(ssize_t)ntokens;  
}



static bool Parser_ValidRel(ssize_t offset) {
  return (pc+offset)>=0 && (pc+offset)<(ssize_t)ntokens;  
}



static bool Parser_Match(TokenType type) {  return Parser_Valid() && Parser_Look()->type==type;
}


static bool Parser_MatchRel(ssize_t offset,TokenType type) {
  return Parser_ValidRel(offset) && Parser_LookRel(offset)->type==type;
}



static Token *Parser_Look() {
  if(pc>=0 && pc<(ssize_t)ntokens) {
    return tokens[pc]; 
  }
  Parser_Error(1,"Error: program counter out of range: %zd",pc);
  return NULL;
}



static Token *Parser_LookRel(ssize_t offset) {
  if(pc+offset>=0 && pc+offset<(ssize_t)ntokens) {
    return tokens[pc+offset]; 
  } else {
    Parser_Error(1,"Error: program counter out of range: %zd",pc+offset);
  }
  return NULL;
}



static int Parser_GetInteger() {
  int r;
  Parser_MatchExpect(TOKENTYPE_INTEGER);
  r=String_ToInteger(Parser_Look()->text);  Parser_Next();
  return r;
}



static char *Parser_GetString() {
  char *r=NULL;
  Parser_MatchExpect(TOKENTYPE_STRING);
  r=(Parser_Look()->text?strdup(Parser_Look()->text):NULL);
  Parser_Next();
  return r;
}



static void Parser_ParseVers(Cite ***cites,size_t *ncites) {
  printf("<vers>\n");  
  
  if(Parser_MatchRel(1,TOKENTYPE_DASH)) {
    bvnum=(size_t)Parser_GetInteger();
    Parser_MatchExpect(TOKENTYPE_DASH);
    Parser_Next();
    evnum=(size_t)Parser_GetInteger();

    vrange=Range_New(bvnum,evnum);
    Range_Append(&vranges,&nvranges,vrange);
    printf("vers: %zu to %zu\n",bvnum,evnum);
  } else {
    bvnum=(size_t)Parser_GetInteger();

    vrange=Range_New(bvnum,bvnum);
    Range_Append(&vranges,&nvranges,vrange);
    printf("vers: %zu\n",bvnum);
  }

  if(Parser_Match(TOKENTYPE_COMMA)) {

    cite=Cite_New(bnum,bname,crange,vranges,nvranges);
    Cite_Append(cites,ncites,cite);

    crange=NULL;

    vranges=NULL;
    nvranges=0;
    
    Parser_Next();
    if(Parser_MatchRel(1,TOKENTYPE_COLON)) {      
      Parser_ParseChapter(cites,ncites);
    } else {
      Parser_ParseVers(cites,ncites);
    }
  }

  printf("</vers>\n");  
}



static void Parser_ParseChapter(Cite ***cites,size_t *ncites) {
  printf("<chapter>\n");
  if(Parser_MatchRel(1,TOKENTYPE_COLON)) {    bcnum=(size_t)Parser_GetInteger();
    printf("chap: %zu\n",bcnum);
    Parser_Next();
    Parser_ParseVers(cites,ncites);
  } else if(Parser_MatchRel(1,TOKENTYPE_DASH)) {
    bcnum=(size_t)Parser_GetInteger();
    Parser_Next();
    ecnum=(size_t)Parser_GetInteger();
    printf("chapter: %zu to %zu\n",bcnum,ecnum);

    crange=Range_New(bcnum,ecnum);
    cite=Cite_New(bnum,bname,crange,vranges,nvranges);
    Cite_Append(cites,ncites,cite);

    crange=NULL;
    vranges=NULL;
    nvranges=0;

  } else {
    bcnum=(size_t)Parser_GetInteger();
    printf("chap: %zu\n",bcnum);

    crange=Range_New(bcnum,bcnum);
    cite=Cite_New(bnum,bname,crange,vranges,nvranges);
    Cite_Append(cites,ncites,cite);

    crange=NULL;
    vranges=NULL;
    nvranges=0;
  }
  printf("</chapter>\n");
}



static void Parser_ParseBook(Cite ***cites,size_t *ncites) {
  printf("<book>\n");

  if(Parser_Match(TOKENTYPE_INTEGER)) {  
    bnum=(size_t)Parser_GetInteger();
  }
  Parser_MatchExpect(TOKENTYPE_STRING);
  bname=Parser_GetString();
  printf("book: %zu %s\n",bnum,bname); 

  Parser_ParseChapter(cites,ncites); 


  Token_Print(Parser_Look());


  if(Parser_Match(TOKENTYPE_SEMICOLON)) {     cite=Cite_New(bnum,bname,crange,vranges,nvranges);
    Cite_Append(cites,ncites,cite);

    crange=NULL;
    vranges=NULL;
    nvranges=0;
    
    Parser_Next();
    Parser_ParseBook(cites,ncites); 
  }

  printf("</book>\n");
}



static void Parser_ParseStart(Cite ***cites,size_t *ncites) {
  Parser_ParseBook(cites,ncites);  
}



void Parser_Parse(Token **toks,size_t ntoks,Cite ***cites,size_t *ncites) {
  
  tokens=toks;
  ntokens=ntoks;
  pc=0;

  Tokens_Print(toks,ntoks);

  Parser_ParseStart(cites,ncites);


  Cites_Print(*cites,*ncites);
}



#endif /* PARSER_IMPLEMENTATION*/



#endif /* PARSER_H */


