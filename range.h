#ifndef RANGE_H
#define RANGE_H



typedef struct Range Range;

struct Range {
	int begin;
	int end;
};



#ifdef RANGE_IMPLEMENTATION



Range *Range_New(int begin,int end) {
	Range *range=NEW1(range);

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



#endif /* RANGE_IMPLEMENTATION */



#endif /* RANGE_H */


