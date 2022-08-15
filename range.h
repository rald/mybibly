#ifndef RANGE_H
#define RANGE_H



typedef struct Range Range;

struct Range {
	size_t begin;
	size_t end;
};



Range *Range_New(int begin,int end);

void Range_Free(Range **range);

void Ranges_Free(Range ***ranges,size_t *nranges);

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



void Range_Free(Range **range) {
  if(range && *range) {
    (*range)->begin=0;
  	(*range)->end=0;
  	free(*range);
  	(*range)=NULL;
	}
}



void Ranges_Free(Range ***ranges,size_t *nranges) {
  if(ranges && *ranges) {
    for(size_t i=0;i<(*nranges);i++) {
      Range_Free(&(*ranges)[i]);
    }  
    free(*ranges);
    (*ranges)=NULL;
    (*nranges)=0;
  }
}



void Range_Append(Range ***ranges,size_t *nranges,Range *range) {
  (*ranges)=realloc(*ranges,sizeof(**ranges)*((*nranges)+1));
  (*ranges)[(*nranges)++]=range;  
}


void Range_Print(Range *range) {
  if(range) {
    printf("RANGE: { begin: %zu end: %zu }",range->begin,range->end);
  } else {
    printf("RANGE: (null)");
  }
}


void Ranges_Print(Range **ranges,size_t nranges) {
  bool f=true;
  if(ranges) {
    printf("RANGES: { ");
    printf("nranges: %zu, ",nranges);
    for(size_t i=0;i<nranges;i++) {
      if(f) f=false; else printf(", ");
      Range_Print(ranges[i]);
    }
    printf(" }\n");
  } else {
    printf("RANGES: (null)");
  }
}



#endif /* RANGE_IMPLEMENTATION */



#endif /* RANGE_H */


