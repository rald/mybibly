#ifndef RANGE_H
#define RANGE_H



typedef struct Range Range;

struct Range {
	size_t begin;
	size_t end;
};



Range *Range_New(int begin,int end);

void Range_Free(void *range);

void Ranges_Free(Range ***ranges,size_t *nranges);

void Range_Append(Range ***ranges,size_t *nranges,Range *range);



#ifdef RANGE_IMPLEMENTATION



Range *Range_New(int begin,int end) {
	Range *range=malloc(sizeof(*range));

	if(range) {

		range->begin=begin;
		
		if(end==0) 
			range->end=begin;		
		else
			range->end=end;
	}

	return range;
}



void Range_Free(void *range) {
	free(range);
}



void Ranges_Free(Range ***ranges,size_t *nranges) {
  for(size_t i=0;i<(*nranges);i++) {
    Range_Free((*ranges)[i]);
    (*ranges)[i]=NULL;
  }  

  free(*ranges);
  (*ranges)=NULL;

  (*nranges)=0;
}



void Range_Append(Range ***ranges,size_t *nranges,Range *range) {
  (*ranges)=realloc(*ranges,sizeof(**ranges)*((*nranges)+1));
  (*ranges)[(*nranges)++]=range;  
}


void Range_Print(Range *range) {
  printf("RANGE: { begin: %zu end: %zu }\n",range->begin,range->end);
}


void Ranges_Print(Range **ranges,size_t nranges) {
  for(size_t i=0;i<nranges;i++) {
    Range_Print(ranges[i]);
  }
  printf("\n");
}

#endif /* RANGE_IMPLEMENTATION */



#endif /* RANGE_H */


