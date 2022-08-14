#ifndef BALT_H
#define BALT_H

typedef struct BAlt BAlt;

struct BAlt {
  char *bname;
	char **alts;
	size_t nalts;
};



BAlt *BAlt_New(char *bname,char **alts,size_t nalts);
void BAlt_Free(void *balt);
void BAlt_Print(BAlt *balt);
void BAlts_Print(BAlt **balts,size_t nbalts);



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




void BAlt_Append(BAlt ***balts,size_t *nbalts,BAlt *balt) {
  (*balts)=realloc(*balts,sizeof(**balts)*((*nbalts)+1));
  (*balts)[(*nbalts)++]=balt;
}




void BAlt_Print(BAlt *balt) {
  if(balt) {
    for(size_t i=0;i<balt->nalts;i++) {
      if(i!=0) printf(",");
      printf("%s",balt->alts[i]);
    }
    printf("\n");
  }
}



void BAlts_Print(BAlt **balts,size_t nbalts) {
  if(balts) {
    for(size_t i=0;i<nbalts;i++) {
      BAlt_Print(balts[i]);
    }
  }
}


#endif /* BALT_IMPLEMENTATION */



#endif /* BALT_H */
