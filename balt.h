#ifndef BALT_H
#define BALT_H

typedef struct BAlt BAlt;

struct BAlt {
  char *bname;
	char **alts;
	size_t nalts;
};



BAlt *BAlt_New(char *bname,char **alts,size_t nalts);
void BAlt_Free(BAlt **balt);
void BAlts_Free(BAlt ***balts,size_t *nbalts);

void BAlt_Print(BAlt *balt);
void BAlts_Print(BAlt **balts,size_t nbalts);

size_t BAlt_Load(char *filename,BAlt ***balts,size_t *nbalts);

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



void BAlt_Free(BAlt **balt) {
  if(balt && *balt) {
    free((*balt)->bname);
    (*balt)->bname=NULL;

    for(size_t i=0;i<(*balt)->nalts;i++) {
      free((*balt)->alts[i]);
      (*balt)->alts[i]=NULL;
    }

    free((*balt)->alts);
    (*balt)->alts=NULL;
    (*balt)->nalts=0;
      
    free(*balt);
  }
}


void BAlts_Free(BAlt ***balts,size_t *nbalts) {
  if(balts && *balts) {
    for(size_t i=0;i<(*nbalts);i++) {
      BAlt_Free(&(*balts)[i]);
    }
  }
  free(*balts);
  (*balts)=NULL;
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


size_t BAlt_Load(char *filename,BAlt ***balts,size_t *nbalts) {

  FILE  *fin=NULL;

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;


  if((fin=fopen(filename,"rt"))) {

  	while((rlen=getline(&line,&llen,fin)>0)) {

  		String_Strrnl(line);

  		BAlt *balt=BAlt_New(NULL,NULL,0);
  						
  		if(String_Strtok(line,",",-1,&balt->alts,&balt->nalts)>0) {

  			balt->bname=strdup(balt->alts[0]);

  			BAlt_Append(balts,nbalts,balt);

  		}

      free(line);
      line=NULL;
      llen=0;
      rlen=0;

  	}			
  }

  return *nbalts;
}




#endif /* BALT_IMPLEMENTATION */



#endif /* BALT_H */
