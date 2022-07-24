#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>



#define LINE_MAX 4096




char *mytrim(char *s) {
    char *p,*q;
    p=s; q=s;
    while(isspace(*q)) ++q;
    while(*q) *p++ = *q++;
    *p=0;
    while(p>s && isspace(*--p)) *p=0;
	return s;
}



char *mystrsub(char *s,int b,int l) {
	char *r,*pr,*ps;
	int i;

	r=malloc(sizeof(char)*(l+1));

	pr=r;
	ps=s+b;
	i=l;
	
	while(*ps && (l==0 || i--)) {
		*pr=*ps;
		pr++;
		ps++;
	}
	*pr=0;

	return r;
}




char *myskip(char *s,char *d) {
	char *pb,*ps;
	ps=0;
	pb=strstr(s,d);
	if(pb) {
		ps=pb+strlen(d);
		*pb=0;
	}
	return ps;
}



void mytokadd(char ***t,int *l,char *s) {
	(*t)=realloc((*t),sizeof(char*)*((*l)+1));
	(*t)[(*l)++]=strdup(s);
}



void mytokfree(char ***toks,int *ntoks) {
	int i;
	i=0;
	while(i<(*ntoks)) {
		free((*toks)[i]);
		(*toks)[i]=0;
		i++;
	}
	free(*toks);
	(*toks)=0;
	(*ntoks)=0;
}



int mystrtok(char *s,char *d,int n,char ***t,int *l) {
	char *ps,*tk;
	int quit;

	quit=0;

	ps=s;

	while(!quit) {
		if((n==-1 || *l<n) && ps) {
			tk=ps;
			ps=myskip(ps,d);
			mytokadd(t,l,tk);
		} else {
			if(ps) mytokadd(t,l,ps);
			quit=1;
		}
	}
		
	return *l;
}




int main() {

	char line[LINE_MAX];

	char **toks=NULL;
	int ntoks=0;

	int i;

	char *book;
	int cnum;
	int vnum;
	char *text;

	FILE *fp;

	fp=fopen("adb.vpl","rt");

	while(fgets(line,LINE_MAX,fp)) {

		char *p=strchr(line,'\n');
		if(p) *p='\0';
			
		if(mystrtok(line,"|",3,&toks,&ntoks)==4) {	

			book=toks[0];
			cnum=atoi(toks[1]);
			vnum=atoi(toks[2]);
			text=toks[3];

			printf("%s %d:%d -> %s\n\n",book,cnum,vnum,text);

			mytokfree(&toks,&ntoks);
		}

	}

	return 0;
}
