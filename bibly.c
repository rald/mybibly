#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>



int LINE_MAX;



int myisspace(char c) {
	return c==' ' || c=='\t' || c=='\n';
}



int mystrlen(char *s) {
	int n;
	n=0;
	while(s[n]) n++;
	return n;
}



int myfgetc(int fd) {
	char c;
	read(fd,&c,1);
	return c;
}



int mygetline(char *b,int s,int fd) {
	int l;
	char c;
	l=0;
	while((c=myfgetc(fd))!='\n' && l<s-1) {
		b[l]=c;
		l++;
	}
	b[l]=0;
	return l;
}



int mystrequ(char *x,char *y) {    
	char *px,*py;
	px=x; py=y;
	while (*px && *py) {
        if (*px != *py) return 0;
        px++;
        py++;
    }
    return *py==0;
}



char *mystrstr(char *x,char *y) {
	char *px,*py;
	px=x; py=y;
    while (*px) {
        if (*px==*py && mystrequ(px,py)) {
            return px;
        }
        px++;
    }
    return 0;
}



char *mytrim(char *s) {
    char *p,*q;
    p=s; q=s;
    while(myisspace(*q)) ++q;
    while(*q) *p++ = *q++;
    *p=0;
    while(p>s && myisspace(*--p)) *p=0;
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
	pb=mystrstr(s,d);
	if(pb) {
		ps=pb+mystrlen(d);
		*pb=0;
	}
	return ps;
}


char *mystrncpy(char *x,char *y,int n) {
	char *px,*py;
	px=x; py=y;
	while(*py && n) {
		*px=*py;
		px++;
		py++;
		n--;
	}
	*px=0;
	return x;
}


char *mystrdup(char *s) {
	char *r;
	int n;
	n=mystrlen(s);
	r=malloc(sizeof(char)*(n+1));
	mystrncpy(r,s,n);
	return r;
}



void tokadd(char ***t,int *l,char *s) {
	(*t)=realloc((*t),sizeof(char*)*((*l)+1));
	(*t)[(*l)++]=mystrdup(s);
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
			tokadd(t,l,tk);
		} else {
			if(ps) tokadd(t,l,ps);
			quit=1;
		}
	}
		
	return *l;
}




int main() {

	int fd;
	char *line;


	char **toks;
	int ntoks;

	int i;

	char *book;
	int cnum;
	int vnum;
	char *text;


	LINE_MAX=65536;

	line=malloc(sizeof(char)*LINE_MAX);

	fd=open("kjv.vpl",0);

	mygetline(line,LINE_MAX,fd);

	toks=0;
	ntoks=0;
		
	if(mystrtok(line,"|",3,&toks,&ntoks)==4) {	
		book=toks[0];
		text=toks[3];

		sscanf(toks[1],"%d",&cnum);
		sscanf(toks[2],"%d",&vnum);

		printf("BOOK: %s\n",book);
		printf("CNUM: %d\n",cnum);
		printf("VNUM: %d\n",vnum);
		printf("TEXT: %s\n",text);
	}
	

	return 0;
}
