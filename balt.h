#ifndef BALT_H
#define BALT_H

typedef struct BAlt BAlt;

struct BAlt {
  char *bname;
	char **alts;
	size_t nalts;
};



#ifdef BALT_IMPLEMENTATION


BAlt *BAlt_New(char *bname,char **alts,size_t nalts) {
  BAlt *balt=malloc(sizeof(*balt));
  if(balt) {
    balt->bname=bname;
    balt->alts=alts;
    balt->nalts=nalts;
  }
  return balt; 
}

void BAlt_Free(void *balt) {
  free(((BAlt*)balt)->bname);
  ((BAlt*)balt)->bname=NULL;

  for(size_t i=0;i<((BAlt*)balt)->nalts;i++) {
    free(((BAlt*)balt)->alts[i]);
    ((BAlt*)balt)->alts[i]=NULL;
  }

  free(((BAlt*)balt)->alts);
  ((BAlt*)balt)->alts=NULL;
  ((BAlt*)balt)->nalts=0;
    
  free(balt);
}



#endif /* BALT_IMPLEMENTATION */



#endif /* BALT_H */
