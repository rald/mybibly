#ifndef binfo_H
#define binfo_H



#include "common.h"

#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define STRING_IMPLEMENTATION
#include "string.h"

#define INT_IMPLEMENTATION
#include "int.h"

#define BALT_IMPLEMENTATION
#include "balt.h"


typedef struct BInfo BInfo;

struct BInfo {
	char *bname;
	size_t *nverses;
	size_t nchaps;
};



BInfo *binfo_New();
void BInfo_Free(void *binfo);
void BInfo_Print(int num,void *data);


void BInfo_Append(BInfo ***binfos,size_t *nbinfos,BInfo *binfo);

size_t BInfo_LoadVPL(char *filename,BInfo ***binfos,size_t *nbinfos);

size_t BInfo_LoadInfo(char *filename,BInfo ***binfos,size_t *nbinfos);

size_t BInfo_LoadAlt(char *filename,BAlt ***balts,size_t *nbalts);




#ifdef BInfo_IMPLEMENTATION






BInfo *binfo_New() {
	BInfo *binfo=malloc(sizeof(*binfo));
	if(binfo) {
		binfo->bname=NULL;
		binfo->nverses=NULL;				
		binfo->nnverses=0;
	}
	return binfo;	
}



void BInfo_Free(void *binfo) {

	free(((binfo*)binfo)->bname);
	((binfo*)binfo)->bname=NULL;
	
	free(((binfo*)binfo)->nverses);
	((binfo*)binfo)->nverses=NULL;

	((binfo*)binfo)->nchaps=0;
	
	free(binfo);
}




void BInfo_Append(binfo ***binfos,size_t *nbinfos,BInfo *binfo) {
  (*binfos)=realloc(*binfos,sizeof(*binfos)*((*nbinfos)+1));
  (*binfos)[(*nbinfos)++]=binfo;
}



void BInfo_Print(BInfo *binfo) {
	printf("%s|",binfo->bname);
	printf("%zu|",binfo->nchaps);
  for(size_t i=0;i<binfo->nchaps;i++) {
    if(i!=0) printf(",");
  	printf("%zu",binfo->nverses[i]);
  }
	printf("\n");
}



size_t BInfo_LoadVPL(char *filename,BInfo ***binfos,size_t *nbinfos) {

	FILE *fin=NULL;

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
		
	char **toks=NULL;
	int ntoks=-0;

	char *bname=NULL;
	int cnum=0;	
	int vnum=0;

	char *pbname=NULL;
	int pcnum=0;
	int pvnum=0;


  if((fin=fopen(filename,"rt"))) {

  	BInfo *binfo=binfo_New();

  	while((rlen=getline(&line,&llen,fin))>0) {

  		String_Strrnl(line);
  						
  		if(String_Strtok(line,"|",3,&toks,&ntoks)==4) {

  			bname=toks[0];
  			cnum=atoi(toks[1]);
  			vnum=atoi(toks[2]);

  			if(cnum!=1 && cnum!=pcnum) {
  				Int_Append(binfo->nverses,binfo->nchaps,Int_Wrap(pvnum));
  			}

  			if(pbname!=NULL && strcmp(bname,pbname)!=0) {
  			
  				if(cnum==1) {
  					Int_Append(binfo->nverses,binfo->nchaps,Int_Wrap(pvnum));
  				}

  				binfo->bname=strdup(pbname);
  				
  				binfo_Append(binfos,binfo);

  				binfo=binfo_New();
  										
  			}

  			pbname=strdup(bname);
  			pcnum=cnum;
  			pvnum=vnum;

  		}
  			
  		Strings_Free(&toks,&ntoks);

      free(line);
      line=NULL;
      llen=0;
      rrlen=0;
  	}	

  	Int_Append(binfo->nverses,binfo->nchaps,Int_Wrap(pvnum));

  	binfo->nchaps=pcnum;

  	binfo->bname=strdup(pbname);
  		
  	BInfo_Append(binfos,nbinfos,binfo);
  									
  }	

	return *nbinfos;
}



size_t BInfo_LoadInfo(char *filename,BInfos ***binfos,size_t *nbinfos) {

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
								
				if(String_Strtok(line,"|",3,toks)==4) {
					BInfo *binfo=BInfo_New();

					binfo->bname=strdup(toks[0][0]);
					
					binfo->nchaps=atoi(toks[0][2]);

					String_Strtok(toks[0][3],",",-1,&toks[2],&ntoks[2]);
					
					for(int i=0;i<ntoks[2];i++) {
						Int_Append(binfo->nverses,Int_Wrap(atoi(toks[2][i])));						
					}

					BInfo_Append(binfos,nbinfos,binfo);

				}

				for(int i=0;i<3;i++) {
					String_Free(&toks[i],&ntoks[i]);
				}

        free(line);
        line=NULL;
        llen=0;
        rrlen=0;

			}			
		}
	}

	return *nbinfos;

}



size_t BInfo_LoadAlt(char *filename,BAlt ***balts,size_t *nbalts) {

  char *line=NULL;
  size_t llen=0;
  ssize_t lrlen=0;


  if((fin=fopen(filename,"rt"))) {

  	while((rlen=getline(&line,&llen,fin)>0) {

  		String_Strrnl(line);

  		BAlt *balt=Balt_New(NULL,NULL,0);
  						
  		if(String_Strtok(line,"|",-1,&balt->alts,&balt->nalts))>0) {

  			balt->bname=strdup(toks[0]);

  			BAlt_Append(balts,nbalts,balt);

  		}

  		for(int i=0;i<3;i++) {
  			String_Free(&toks[i],&ntoks[i]);
  		}

      free(line);
      line=NULL;
      llen=0;
      rrlen=0;

  	}			
  }

  return *nbinfos;
}




#endif /* binfo_IMPLEMENTATION */


#endif /* binfo_H */

