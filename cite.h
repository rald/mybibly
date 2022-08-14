#ifndef CITE_H
#define CITE_H



#include "common.h"

#include "range.h"

#include "passage.h"



typedef struct Cite Cite;

struct Cite {
	char *bname;
	Range *crange;
	Range **vranges;
	size_t nvranges;
};



Cite *Cite_New(char *bname,Range *crange,Range **vranges,size_t nvranges);

void Cite_Free(void *cite);

void Cites_Free(Cite **cites,size_t *ncites);

void Cite_Append(Cite ***cites,size_t *ncites,Cite *cite);

void Cite_Print(Cite *cite);
void Cites_Print(Cite **cites,size_t ncites);



#ifdef CITE_IMPLEMENTATION



Cite *Cite_New(char *bname,Range *crange,Range **vranges,size_t nvranges) {
	Cite *cite=NEW1(cite);
	if(cite) {
		cite->bname=bname?strdup(bname):NULL;
		cite->crange=crange;
		cite->vranges=vranges;
		cite->nvranges=nvranges;
	}
	return cite;
}



void Cite_Free(void *cite) {
	free(((Cite*)cite)->bname);
	((Cite*)cite)->bname=NULL;

	Range_Free(((Cite*)cite)->crange);
	((Cite*)cite)->crange=NULL;

  Ranges_Free(
    &((Cite*)cite)->vranges,
    &((Cite*)cite)->nvranges
  );
	
	free(cite);
}



void Cites_Free(Cite **cites,size_t *ncites) {
  for(size_t i=0;i<(*ncites);i++) {
    Cite_Free(cites[i]);
    cites[i]=NULL;    
  }
  free(cites);
  cites=NULL;  
}



void Cite_Append(Cite ***cites,size_t *ncites,Cite *cite) {
  (*cites)=realloc(*cites,sizeof(**cites)*((*ncites)+1));
  (*cites)[(*ncites)++]=cite;  
}




void Cite_Print(Cite *cite) {

  if(cite) {

    printf("book: %s\n",cite->bname);
     
    if(cite->crange) {
      if(cite->crange->begin==cite->crange->end) {
        printf("chap: %zu\n",cite->crange->begin);
      } else {
        printf("chap: %zu to %zu\n",cite->crange->begin,cite->crange->end);
      }
    }

    Ranges_Print(cite->vranges,cite->nvranges);
    
  } else {
    printf("(null)\n");
  }
}



void Cites_Print(Cite **cites,size_t ncites) {
  if(cites) {
    printf("ncites: %zu\n",ncites);
    for(size_t i=0;i<ncites;i++) {
      Cite_Print(cites[i]);
    }
    printf("\n");
  } else {
    printf("(null)\n");
  }
}



void Cite_Parse(Cite *cite) {
  if(cite->crange->begin<cite->crange->end) {
    for(size_t i=cite->crange->begin;i<=cite->crange->end;i++) {
            
    }
  } else if(cite->crange->begin==cite->crange->end) {
    if(cite->nvranges>0) {
      for(size_t j=0;j<=cite->nvranges;j++) {
        for(  size_t i=cite->vranges[j]->begin;
              i<=cite->vranges[j]->end;i++) {
          if(cite->bnum!=0) printf("%zu %s %zu:%zu",cite->bnum,cite->bname,cite->crange->begin,i);
        }
      }
    }
  }
}






#endif /* CITE_IMPLEMETATION */



#endif /* CITE_H */


