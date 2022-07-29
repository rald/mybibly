#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define STRING_MAX 1024

int main(int argc,char **argv) {

	FILE *fin;

	char line[STRING_MAX];
	char word[STRING_MAX];

	char **toks[2];
	int ntoks[2];

	if(argc<2) {
		printf("Syntax: %s filename\n",argv[0]);
		return 1;
	}
	
	if((fin=fopen(argv[1],"rt"))==NULL) {
		perror(argv[1]);
		return 2;
	}


	toks[0]=toks[1]=NULL;
	ntoks[0]=ntoks[1]=0;

	while(fgets(line,STRING_MAX-2,fin)) {
		mystrrnl(line);
		if(mystrtok(line,"|",3,&toks[0],&ntoks[0])==4) {
			mystrtok(toks[0][2]," ",3,&toks[1],&ntoks[1]);
			for(int i=0;i<ntoks[1];i++) {
				printf("%s\n",toks[1][i]);
			}
			mytokfree(&toks[1],&ntoks[1]);
		}
		mytokfree(&toks[0],&ntoks[0]);
	}

	return 0;
}
