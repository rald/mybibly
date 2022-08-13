#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STRING_MAX 1024

typedef struct Pair Pair;

struct Pair {
	long n;
	char *s;
};


char *skip(char *s,char c) {

	while(*s && *s!=c) s++;
	if(*s==c) {
		*s=0;
		s++;
	}

	return s;
}


int cmp(const void *a,const void *b) {
	Pair *l=*(Pair**)a;
	Pair *r=*(Pair**)b;
	if(l->n > r->n) return -1;
	else if(l->n < r->n) return 1;
	else if(strlen(l->s) > strlen(r->s)) return -1;
	else if(strlen(l->s) < strlen(r->s)) return 1;
	return 0;
}


void swap(char *x,char *y) {
	char t=*x;
	*x=*y;
	*y=t;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(str+start, str+end);
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
		char *code="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int i = 0;
    bool isNegative = false;
    
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = code[rem];
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}


int main() {

	char line[STRING_MAX];
	char *tok[2];
	
	Pair **pairs=NULL;
	long npairs=0;

	char hex[STRING_MAX];

	while(fgets(line,STRING_MAX-2,stdin)) {
		char *p=strchr(line,'\n'); if(p) *p='\0';
		tok[0]=line;
		tok[1]=skip(line,'|');

		Pair *pair=malloc(sizeof(*pair));	

		pair->n=atol(tok[0]);
		pair->s=strdup(tok[1]);

		pairs=realloc(pairs,sizeof(*pairs)*(npairs+1));
		pairs[npairs++]=pair;		
	}

	qsort(pairs,npairs,sizeof(*pairs),cmp);

	for(long i=0;i<npairs;i++) {
		printf("%ld|%s\n",pairs[i]->n,pairs[i]->s);
	}

}
