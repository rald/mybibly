#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define STRING_MAX 1024

int main(int argc,char **argv) {

	FILE *fin;

	char line[STRING_MAX];
	char word[STRING_MAX];

	char **toks=NULL;
	int ntoks=0;

	char *p;

	if(argc<2) {
		printf("Syntax: %s filename\n",argv[0]);
		return 1;
	}
	
	if((fin=fopen(argv[1],"rt"))==NULL) {
		perror(argv[1]);
		return 2;
	}

	while(fgets(line,STRING_MAX-2,fin)) {
		mystrrnl(line);
		int i=0;
		while(i<strlen(line)) {
			if(isalnum(line[i])) {
				while(isalnum(line[i]) || line[i]=='\'') {
					putchar(line[i]);
					i++;
				}
				putchar('\n');
			} else if(isspace(line[i])) {
				while(isspace(line[i])) i++;
			} else {
				putchar(line[i]);
				putchar('\n');
				i++;
			}
		}
		mytokfree(&toks,&ntoks);
	}

	return 0;
}
