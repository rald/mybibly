In file included from bibly.c:21:
In file included from ./parser.h:18:
./cite.h:138:20: error: no member named 'bnum' in 'struct Cite'
          if(cite->bnum!=0) printf("%zu %s %zu:%zu",cite->bnum,cite->bname,cite->crange->begin,i);
             ~~~~  ^
./cite.h:138:59: error: no member named 'bnum' in 'struct Cite'
          if(cite->bnum!=0) printf("%zu %s %zu:%zu",cite->bnum,cite->bname,cite->crange->begin,i);
                                                    ~~~~  ^
In file included from bibly.c:21:
./parser.h:283:45: error: too many arguments to function call, expected 4, have 5
    cite=Cite_New(bnum,bname,crange,vranges,nvranges);
         ~~~~~~~~                           ^~~~~~~~
./cite.h:42:7: note: 'Cite_New' declared here
Cite *Cite_New(char *bname,Range *crange,Range **vranges,size_t nvranges) {
      ^
In file included from bibly.c:21:
./parser.h:289:45: error: too many arguments to function call, expected 4, have 5
    cite=Cite_New(bnum,bname,crange,vranges,nvranges);
         ~~~~~~~~                           ^~~~~~~~
./cite.h:42:7: note: 'Cite_New' declared here
Cite *Cite_New(char *bname,Range *crange,Range **vranges,size_t nvranges) {
      ^
In file included from bibly.c:21:
./parser.h:307:8: warning: implicit declaration of function 'String_CaseIndexOf' is invalid in C99 [-Wimplicit-function-declaration]
    if(String_CaseIndexOf(book,balts->alts[i])) {
       ^
./parser.h:307:37: error: member reference base type 'BAlt *' (aka 'struct BAlt *') is not a structure or union
    if(String_CaseIndexOf(book,balts->alts[i])) {
                               ~~~~~^ ~~~~
./parser.h:309:24: error: member reference base type 'BAlt *' (aka 'struct BAlt *') is not a structure or union
      book=strdup(balts->alts[0]);      
                  ~~~~~^ ~~~~
1 warning and 6 errors generated.
make: *** [Makefile:2: bibly] Error 1
