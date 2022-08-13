#ifndef CITE_H
#define CITE_H



#include "common.h"

#include "range.h"

#include "passage.h"



typedef struct Cite Cite;

struct Cite {
  size_t bnum;
	char *bname;
	Range *rchap;
	Range **rverses;
	size_t nrverses;
};



Cite *Cite_New(size_t bnum,char *bname,Range *rchap,Range **rverses,size_t nrverses);

void Cite_Free(void *cite);

void Cites_Free(Cite **cites,size_t *ncites);

void Cite_Append(Cite ***cites,size_t *ncites,Cite *cite);

void Cite_Print(Cite *cite);
void Cites_Print(Cite **cites,size_t ncites);



#ifdef CITE_IMPLEMENTATION



Cite *Cite_New(size_t bnum,char *bname,Range *rchap,Range **rverses,size_t nrverses) {
	Cite *cite=NEW1(cite);
	if(cite) {
	  cite->bnum=bnum;
		cite->bname=bname?strdup(bname):NULL;
		cite->rchap=rchap;
		cite->rverses=rverses;
		cite->nrverses=nrverses;
	}
	return cite;
}



void Cite_Free(void *cite) {
	free(((Cite*)cite)->bname);
	((Cite*)cite)->bname=NULL;

	Range_Free(((Cite*)cite)->rchap);
	((Cite*)cite)->rchap=NULL;

  Ranges_Free(
    &((Cite*)cite)->rverses,
    &((Cite*)cite)->nrverses
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
    if(cite->bnum) 
      printf("book: %zu %s\n",cite->bnum,cite->bname);
    else 
      printf("book: %s\n",cite->bname);
    if(cite->rchap) {
      printf("bchap: %zu\n",cite->rchap->begin);
      printf("echap: %zu\n",cite->rchap->end);
    }
    if(cite->rverses) {
      printf("nrverses: %zu\n",cite->nrverses);
      for(size_t i=0;i<cite->nrverses;i++) {
        if(cite->rverses[i]) {
          printf("%zu -> begin: %zu end: %zu\n",i+1,cite->rverses[i]->begin,cite->rverses[i]->end);
        }
      }   
    }
    printf("\n"); 
  } else {
    printf("(null)\n");
  }
}



void Cites_Print(Cite **cites,size_t ncites) {
  if(cites) {
    printf("ncites: %zu\n\n",ncites);
    for(size_t i=0;i<ncites;i++) {
      Cite_Print(cites[i]);
    }
    printf("\n");
  } else {
    printf("(null)\n");
  }
}



void Cite_Parse(Cite *cite) {
  if(cite->rchap->begin<cite->rchap->end) {
    for(size_t i=cite->rchap->begin;i<=cite->rchap->end;i++) {
            
    }
  } else if(cite->rchap->begin==cite->rchap->end) {
    if(cite->nrverses>0) {
      for(size_t j=0;j<=cite->nrverses;j++) {
        for(  size_t i=cite->rverses[j]->begin;
              i<=cite->rverses[j]->end;i++) {
          if(cite->bnum!=0) printf("%zu %s %zu:%zu",cite->bnum,cite->bname,cite->rchap->begin,i);
        }
      }
    }
  }
}






#endif /* CITE_IMPLEMETATION */



#endif /* CITE_H */


