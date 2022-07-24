#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ezxml.h"


char *strchg(char *s) {

	char num[256]="",nam[256]="";

	sscanf(s,"%s %s",num,nam);

	if(strcmp(num,"I")==0) {
		strcpy(num,"1");
		sprintf(s,"%s %s",num,nam);
	}
	
	if(strcmp(num,"II")==0) {
		strcpy(num,"2");	
		sprintf(s,"%s %s",num,nam);
	}

	if(strcmp(num,"III")==0) {
		strcpy(num,"3");
		sprintf(s,"%s %s",num,nam);
	}

	return s;
}


int main() {

	ezxml_t bible = ezxml_parse_file("bbe.xml"),book,chap,vers;

	const char *bname;
	const char *cnum;
	const char *vnum;
	const char *text;


		
	for (book = ezxml_child(bible, "BIBLEBOOK"); book; book = book->next) {

		for (chap = ezxml_child(book, "CHAPTER"); chap; chap = chap->next) {

			for (vers = ezxml_child(chap, "VERS"); vers; vers = vers->next) {


				bname = ezxml_attr(book, "bname");
				cnum = ezxml_attr(chap, "cnumber");
				vnum = ezxml_attr(vers, "vnumber");
				text=ezxml_txt(vers);


				char bname2[32];

				strcpy(bname2,bname);

				strchg(bname2);
				
				printf("%s|%s|%s|%s\n",bname2,cnum,vnum,text);


			}

		}

	}

	ezxml_free(book);

	return 0;
}
