#ifndef binfo_H
#define binfo_H



#include "common.h"

#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define STRING_IMPLEMENTATION
#include "string.h"

#define SIZE_T_IMPLEMENTATION
#include "size_t.h"

#define BALT_IMPLEMENTATION
#include "balt.h"



typedef struct BInfo BInfo;

struct BInfo {
	char *bname;
	size_t **nverses;
	size_t nchaps;
};



BInfo *binfo_New();
void BInfo_Free(void *binfo);
void BInfo_Print(BInfo *binfo);

void BInfo_Append(BInfo ***binfos,size_t *nbinfos,BInfo *binfo);

size_t BInfo_LoadVPL(char *filename,BInfo ***binfos,size_t *nbinfos);

size_t BInfo_LoadInfo(char *filename,BInfo ***binfos,size_t *nbinfos);

size_t BInfo_LoadAlt(char *filename,BAlt ***balts,size_t *nbalts);




#ifdef BINFO_IMPLEMENTATION






BInfo *BInfo_New() {
	BInfo *binfo=malloc(sizeof(*binfo));
	if(binfo) {
		binfo->bname=NULL;
		binfo->nverses=NULL;
		binfo->nchaps=0;				
	}
	return binfo;	
}



void BInfo_Free(void *binfo) {

	free(((BInfo*)binfo)->bname);
	((BInfo*)binfo)->bname=NULL;
	
	free(((BInfo*)binfo)->nverses);
	((BInfo*)binfo)->nverses=NULL;

	((BInfo*)binfo)->nchaps=0;
	
	free(binfo);
}




void BInfo_Append(BInfo ***binfos,size_t *nbinfos,BInfo *binfo) {
  (*binfos)=realloc(*binfos,sizeof(*binfos)*((*nbinfos)+1));
  (*binfos)[(*nbinfos)++]=binfo;
}



void BInfo_Print(BInfo *binfo) {
	printf("%s|",binfo->bname);
	printf("%zu|",binfo->nchaps);
  for(size_t i=0;i<binfo->nchaps;i++) {
    if(i!=0) printf(",");
  	printf("%zu",Size_t_Unwrap(binfo->nverses[i]));
  }
	printf("\n");
}



size_t BInfo_LoadVPL(char *filename,BInfo ***binfos,size_t *nbinfos) {

	FILE *fin=NULL;

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
		
	char **toks=NULL;
	size_t ntoks=-0;

	char *bname=NULL;
	size_t cnum=0;	
	size_t vnum=0;

	char *pbname=NULL;
	size_t pcnum=0;
	size_t pvnum=0;


  if((fin=fopen(filename,"rt"))) {

  	BInfo *binfo=BInfo_New();

  	while((rlen=getline(&line,&llen,fin))>0) {

  		String_Strrnl(line);
  						
  		if(String_Strtok(line,"|",3,&toks,&ntoks)==4) {

  			bname=toks[0];
  			cnum=atoi(toks[1]);
  			vnum=atoi(toks[2]);

  			if(cnum!=1 && cnum!=pcnum) {
  				Size_t_Append(&binfo->nverses,&binfo->nchaps,Size_t_Wrap(pvnum));
  			}

  			if(pbname!=NULL && strcmp(bname,pbname)!=0) {
  			
  				if(cnum==1) {
  					Size_t_Append(&binfo->nverses,&binfo->nchaps,Size_t_Wrap(pvnum));
  				}

  				binfo->bname=strdup(pbname);
  				
  				BInfo_Append(binfos,nbinfos,binfo);

  				binfo=BInfo_New();
  										
  			}

  			pbname=strdup(bname);
  			pcnum=cnum;
  			pvnum=vnum;

  		}
  			
  		Strings_Free(&toks,&ntoks);

      free(line);
      line=NULL;
      llen=0;
      rlen=0;
  	}	

  	Size_t_Append(&binfo->nverses,&binfo->nchaps,Size_t_Wrap(pvnum));

  	binfo->nchaps=pcnum;

  	binfo->bname=strdup(pbname);
  		
  	BInfo_Append(binfos,nbinfos,binfo);
  									
  }	

	return *nbinfos;
}



size_t BInfo_LoadInfo(char *filename,BInfo ***binfos,size_t *nbinfos) {

	FILE *fin=NULL;

	char *line;
	size_t llen=0;
	ssize_t rlen=0;
		

	char **toks[3];
	size_t ntoks[3];


	for(int i=0;i<3;i++) {
		toks[i]=NULL;
		ntoks[i]=0;
	}
	

  if((fin=fopen(filename,"rt"))) {

  	while((rlen=getline(&line,&llen,fin))>0) {
  		String_Strrnl(line);
  						
  		if(String_Strtok(line,"|",3,&toks[0],&ntoks[0])==4) {
  			BInfo *binfo=BInfo_New();

  			binfo->bname=strdup(toks[0][0]);
  			
  			binfo->nchaps=atoi(toks[0][2]);

  			String_Strtok(toks[0][3],",",-1,&toks[2],&ntoks[2]);
  			
  			for(size_t i=0;i<ntoks[2];i++) {
  				Size_t_Append(&binfo->nverses,&binfo->nchaps,Size_t_Wrap(atoi(toks[2][i])));						
  			}

  			BInfo_Append(binfos,nbinfos,binfo);

  		}

  		for(size_t i=0;i<3;i++) {
  			Strings_Free(&toks[i],&ntoks[i]);
  		}

        free(line);
        line=NULL;
        llen=0;
        rlen=0;

  	}			
  }


	return *nbinfos;

}



#endif /* BINFO_IMPLEMENTATION */



#endif /* BINFO_H */

