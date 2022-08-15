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

void Cite_Free(Cite **cite);

void Cites_Free(Cite ***cites,size_t *ncites);

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



void Cite_Free(Cite **cite) {

  if(cite && *cite) {
  	if((*cite)->bname) {
      free((*cite)->bname);
      (*cite)->bname=NULL;
  	}

  	Range_Free(&(*cite)->crange);

    Ranges_Free(
      &(*cite)->vranges,
      &(*cite)->nvranges
    );
  	
  	free(cite);
  	cite=NULL;

	}

}



void Cites_Free(Cite ***cites,size_t *ncites) {

  if(cites && *cites) {

    for(size_t i=0;i<(*ncites);i++) {
      Cite_Free(&(*cites)[i]);
      (*cites)[i]=NULL;    
    }

    free(*cites);
    (*cites)=NULL;  

  }
}



void Cite_Append(Cite ***cites,size_t *ncites,Cite *cite) {
  (*cites)=realloc(*cites,sizeof(**cites)*((*ncites)+1));
  (*cites)[(*ncites)++]=cite;  
}




void Cite_Print(Cite *cite) {

  if(cite) {

    printf("CITE: { ");

    printf(" book: %s, ",cite->bname);
     
    if(cite->crange) {
      if(cite->crange->begin==cite->crange->end) {
        printf("chap: %zu, ",cite->crange->begin);
      } else {
        printf("chap: %zu to %zu, ",cite->crange->begin,cite->crange->end);
      }
    }

    if(cite->vranges) {
      Ranges_Print(cite->vranges,cite->nvranges);
    }

    printf(" }\n");
    
  } else {
    printf("CITE: (null)\n");
  }
}



void Cites_Print(Cite **cites,size_t ncites) {
  if(cites) {
    printf("CITES: { ");
    printf("ncites: %zu, ",ncites);
    for(size_t i=0;i<ncites;i++) {
      if(i!=0) printf(", ");
      Cite_Print(cites[i]);
    }
    printf(" }\n");
  } else {
    printf("CITES: (null)\n");
  }
}



void Cites_ToPassages(Cite **cites,size_t ncites,Passage **passages,size_t npassages,Passage ***fpassages,size_t *nfpassages) {
  if(cites && passages) {
    for(size_t k=0;k<npassages;k++) {
      for(size_t j=0;j<ncites;j++) {
        if(
          (cites[j] && passages[k] && cites[j]->bname && passages[k]->bname &&
          !strcasecmp(cites[j]->bname,passages[k]->bname)) &&
          (cites[j]->crange && 
          passages[k]->cnum>=cites[j]->crange->begin && 
          passages[k]->cnum<=cites[j]->crange->end)) {
          for(size_t i=0;i<cites[j]->nvranges;i++) {
            if(
              cites[j]->vranges[i] && 
              passages[k]->vnum>=cites[j]->vranges[i]->begin && 
              passages[k]->vnum<=cites[j]->vranges[i]->end
            ) {
              Passage_Append(fpassages,nfpassages,passages[k]);
            }
          }
        }
      }
    }
  }
}




#endif /* CITE_IMPLEMETATION */



#endif /* CITE_H */


