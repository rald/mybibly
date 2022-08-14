#ifndef STRING_H
#define STRING_H



#define COMMON_IMPLEMENTATION
#include "common.h"

void String_Free(void *data);
void Strings_Free(char ***ss,size_t *nss);
char *String_Append(char **s,char *fmt,...);

char *String_Skip(char *s,char *d);

size_t String_Strtok(char *s,char *d,ssize_t n,char ***tokens,size_t *ntokens);

char *String_Trim(char *s);

char *String_Strrnl(char *s);

ssize_t String_IndexOfChar(char *s,char c);

ssize_t Strings_IndexOf(char **h,size_t nh ,char *n)
ssize_t Strings_CaseIndexOf(char **h,size_t nh ,char *n);

bool String_Match(char str[], char pattern[], int n, int m);

int String_ToInteger(const char *s);



#ifdef STRING_IMPLEMENTATION



static void arrapp(char ***a,size_t *na,char *s);



int String_ToInteger(const char *s) {
  int r=0;
  sscanf(s,"%d",&r);
  return r;
}



void String_Free(void *data) {
  free(data);
}



void Strings_Free(char ***ss,size_t *nss) {  for(size_t i=0;i<(*nss);i++) {
    String_Free((*ss)[i]);
    (*ss)[i]=NULL;
  }
  free(*ss);
  (*ss)=NULL;
  (*nss)=0;
}



static void arrapp(char ***a,size_t *na,char *s) {
  (*a)=realloc(*a,sizeof(**a)*((*na)+1));
  (*a)[(*na)++]=s?strdup(s):NULL;
}



char *String_Copy(char **s,char *fmt,...) {
  va_list args;

  char *b=NULL;
  size_t nb=0;
  size_t ns=0;

  va_start(args,fmt);
    nb=vsnprintf(NULL,0,fmt,args);
  va_end(args);

  b=calloc(nb+1,sizeof(*b));

  va_start(args,fmt);
    vsnprintf(b,nb+1,fmt,args);
  va_end(args);

  if(b) {
    if(*s) {
      free(*s);
    }
    (*s)=strdup(b);
    free(b);
    b=NULL;
  }  

  return s;
}



char *String_Append(char **s,char *fmt,...) {
  va_list args;

  char *b=NULL;
  size_t nb=0;
  size_t ns=0;

  va_start(args,fmt);
    nb=vsnprintf(NULL,0,fmt,args);
  va_end(args);

  b=calloc(nb+1,sizeof(*b));

  va_start(args,fmt);
    vsnprintf(b,nb+1,fmt,args);
  va_end(args);

  if(b && *b) {
    if(*s) {
      ns=strlen(*s)+nb;
      (*s)=realloc(*s,sizeof(**s)*(ns+1));
      strncat(*s,b,ns+1);
    } else {
      (*s)=strdup(b);
    }
    free(b);
    b=NULL;
  }

  return *s;  
}



char *String_Skip(char *s,char *d) {
	char *pb=NULL;
	char *ps=NULL;

	pb=strstr(s,d);
	if(pb) {
		ps=pb+strlen(d);
		*pb=0;
	}
	return ps;
}



size_t String_Strtok(char *s,char *d,ssize_t n,char ***tokens,size_t *ntokens) {

	char *ps,*tk;
	bool quit=false;

	ps=s;
	while(!quit) {
		if(((n==-1 || ((ssize_t)(*ntokens))<n)) && ps) {
			tk=ps;
			ps=String_Skip(ps,d);
			arrapp(tokens,ntokens,tk);
		} else {
			if(ps) arrapp(tokens,ntokens,ps);
			quit=true;
		}
	}
		
	return *ntokens;
}



char *String_Trim(char *s) {
	char *p,*q;
	p=s; q=s;
	while(isspace(*q)) ++q;
	while(*q) *p++ = *q++;
	*p=0;
	while(p>s && isspace(*--p)) *p=0;
	return s;
}



char *String_Strrnl(char *s) {
	char *p=strchr(s,'\n');
	if(p) *p='\0';
	return s;	
}



ssize_t String_IndexOfChar(char *s,char c) {
  ssize_t j=-1;
  for(size_t i=0;s[i];i++) {
    if(s[i]==c) {
      j=i;
      break;
    }
  }
  return j;
}



ssize_t Strings_IndexOf(char **h,size_t nh ,char *n) {
  for(size_t i=0;i<nh;i++) {
    if(strcmp(h[i])==0) {
      return (ssize_t)i;
    }
  }
  return -1;
}




ssize_t Strings_CaseIndexOf(char **h,size_t nh ,char *n) {
  for(size_t i=0;i<nh;i++) {
    if(strcasecmp(h[i])==0) {
      return (ssize_t)i;
    }
  }
  return -1;
}




// Function that matches input str with
// given wildcard pattern
bool String_Match(char str[], char pattern[], int n, int m)
{
    // empty pattern can only match with
    // empty string
    if (m == 0)
        return (n == 0);
 
    // lookup table for storing results of
    // subproblems
    bool lookup[n + 1][m + 1];
 
    // initialize lookup table to false
    memset(lookup, false, sizeof(lookup));
 
    // empty pattern can match with empty string
    lookup[0][0] = true;
 
    // Only '*' can match with empty string
    for (int j = 1; j <= m; j++)
        if (pattern[j - 1] == '*')
            lookup[0][j] = lookup[0][j - 1];
 
    // fill the table in bottom-up fashion
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            // Two cases if we see a '*'
            // a) We ignore ‘*’ character and move
            //    to next  character in the pattern,
            //     i.e., ‘*’ indicates an empty sequence.
            // b) '*' character matches with ith
            //     character in input
            if (pattern[j - 1] == '*')
                lookup[i][j]
                    = lookup[i][j - 1] || lookup[i - 1][j];
 
            // Current characters are considered as
            // matching in two cases
            // (a) current character of pattern is '?'
            // (b) characters actually match
            else if (pattern[j - 1] == '?'
                     || toupper(str[i - 1]) == toupper(pattern[j - 1]))
                lookup[i][j] = lookup[i - 1][j - 1];
 
            // If characters don't match
            else
                lookup[i][j] = false;
        }
    }
 
    return lookup[n][m];
}



#endif /* STRING_IMPLEMENTATION_*/



#endif /* STRING_H */


