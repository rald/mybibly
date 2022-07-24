#include <stdio.h>
#include <stdlib.h>

int LINE_MAX;



int strlen(char *s) {
	int n;

	n=0;
	
	while(s[n]) n++;

	return n;
}



int getc(int fd) {
	char c;
	read(fd,&c,1);
	return c;
}



char *myfgets(char *b,int s,int fd) {
	int l;
	char c;
	l=0;
	while((c=getc(fd))!='\n' && l<s-1) {
		b[l]=c;
		l++;
	}
	b[l]=0;
	return l;
}



int main() {

	int fd;
	char *line;



	LINE_MAX=65536;

	

	line=malloc(sizeof(char)*LINE_MAX);

	fd=open("adb.vpl",0);

	while(myfgets(line,LINE_MAX,fd)) {
		printf("%s\n",line);
	}

	return 0;
}
