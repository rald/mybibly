#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define STRING_MAX 1024

int main(int argc,char **argv) {

	FILE *fin;

	char line[STRING_MAX];
	char prev[STRING_MAX]="";

	char **toks=NULL;
	int ntoks=0;

	char *a;

	if(argc<2) {
		printf("Syntax: %s filename\n",argv[0]);
		return 1;
	}
	
	if((fin=fopen(argv[1],"rt"))==NULL) {
		perror(argv[1]);
		return 2;
	}

	int n=1;
	if(fgets(prev,STRING_MAX-2,fin)) {

		do {

			mystrrnl(prev);

			while((a=fgets(line,STRING_MAX-2,fin))) {

				mystrrnl(line);

				if(strcmp(line,prev)==0) { 
					n++; 
					continue; 
				}
				
				break;
			}
			
			printf("%d|%s\n",n,prev);

			strcpy(prev,line);

			n=1;
			
		} while(a);

	}
	
	return 0;
}
