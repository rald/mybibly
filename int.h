#ifndef INT_H
#define INT_H



void Int_Print(int num,void *data);
void Int_Free(void *data);
int *Int_Wrap(int data);
int Int_Unwrap(int *data);



#ifdef INT_IMPLEMENTATION



void Int_Print(int num,void *data) {
	if(num!=1) printf(",");
	printf("%d",*(int*)data);
}



void Int_Free(void *data) {
	free(data);	
}



int *Int_Wrap(int data) {
	int *result=malloc(sizeof(*result));
	*result=data;
	return result;	
}



int Int_Unwrap(int *data) {
	return *data;	
}



#endif /* INT_IMPLEMENTATION */


#endif /* INT_H */
