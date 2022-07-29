#ifndef BINFO_H
#define BINFO_H


#include "common.h"

#define MYLIB_IMPLEMENTATION
#include "mylib.h"



typedef struct BInfo BInfo;

struct BInfo {
	char *bname;
	Vector *bsnames;
	int nchaps;
	Vector *nverses;
};



BInfo *BInfo_New();
void BInfo_Free(void *bInfo);
void BInfo_Print(int num,void *data);
Vector *BInfo_Load(char *filename);


void String_Print(int num,void *data);
void String_Free(void *data);

void Int_Print(int num,void *data);
void Int_Free(void *data);

int *Int_Wrap(int data);
int Int_Unwrap(int *data);



#ifdef BINFO_IMPLEMENTATION



void String_Print(int num,void *data) {
	if(num!=1) printf(",");
	printf("%s",(char*)data);	
}

void String_Free(void *data) {
	free(data);	
}



void Int_Print(int num,void *data) {
	if(num!=1) printf(",");
	printf("%d",*(int*)data);
}

void Int_Free(void *data) {
	free(data);	
}

int *Int_Wrap(int data) {
	int *result=malloc(sizeof(*result));
	*result=data;
	return result;	
}

int Int_Unwrap(int *data) {
	return *data;	
}



BInfo *BInfo_New() {
	BInfo *bInfo=malloc(sizeof(*bInfo));
	if(bInfo) {
		bInfo->bname=NULL;
		bInfo->bsnames=Vector_New(0,String_Print,String_Free);
		bInfo->nchaps=0;
		bInfo->nverses=Vector_New(0,Int_Print,Int_Free);				
	}
	return bInfo;	
}



void BInfo_Free(void *bInfo) {

	free(((BInfo*)bInfo)->bname);
	((BInfo*)bInfo)->bname=NULL;

	Vector_Free(((BInfo*)bInfo)->bsnames);
	((BInfo*)bInfo)->bsnames=NULL;
	
	((BInfo*)bInfo)->nchaps=0;		
	
	Vector_Free(((BInfo*)bInfo)->nverses);
	((BInfo*)bInfo)->nverses=NULL;
	
	free(bInfo);
}



void BInfo_Print(int num,void *data) {
	printf("%s|",((BInfo*)data)->bname);	
	printf("%d|",((BInfo*)data)->nchaps);	
	Vector_Print(0,((BInfo*)data)->nverses);
}



Vector *BInfo_Load(char *filename) {

	Vector *bInfos=Vector_New(0,BInfo_Print,BInfo_Free);
	FILE *fin=NULL;
	char line[STRING_MAX];

	char **toks=NULL;
	int ntoks=-0;

	char *bname=NULL;
	int cnum=0;	
	int vnum=0;
	char *text;

	char *pbname=NULL;
	int pcnum=0;
	int pvnum=0;


	if(bInfos) {

		if((fin=fopen(filename,"rt"))==NULL) {

			Vector_Free(bInfos);
			bInfos=NULL;
			
		} else {

			BInfo *bInfo=BInfo_New();

	
			while(fgets(line,STRING_MAX-2,fin)!=NULL) {

				mystrrnl(line);
								
				if(mystrtok(line,"|",3,&toks,&ntoks)==4) {

					bname=toks[0];
					cnum=atoi(toks[1]);
					vnum=atoi(toks[2]);
					text=toks[3];

					//printf("%s %d:%d -> %s\n\n",bname,cnum,vnum,text);

					if(cnum!=1 && cnum!=pcnum) {
						Vector_Append(bInfo->nverses,Int_Wrap(pvnum));
						bInfo->nchaps=pcnum;
					}



					if(pbname!=NULL && strcmp(bname,pbname)!=0) {
					
						if(cnum==1) {
							Vector_Append(bInfo->nverses,Int_Wrap(pvnum));
							bInfo->nchaps=pcnum;
						}

						bInfo->bname=strdup(pbname);
						
						Vector_Append(bInfos,bInfo);

						bInfo=BInfo_New();
												
					}

					pbname=strdup(bname);
					pcnum=cnum;
					pvnum=vnum;

				}
					
				mytokfree(&toks,&ntoks);

			}	

			Vector_Append(bInfo->nverses,Int_Wrap(pvnum));

			bInfo->nchaps=pcnum;

			bInfo->bname=strdup(pbname);
				
			Vector_Append(bInfos,bInfo);
											
		}	
	}
	
	return bInfos;
}


#endif /* BINFO_IMPLEMENTATION */


#endif /* BINFO_H */

