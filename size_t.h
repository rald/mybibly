#ifndef SIZE_T_H
#define SIZE_T_H




void Size_t_Append(size_t ***sts,size_t *nsts,size_t *data);

void Size_t_Print(size_t *data);
void Size_t_Free(void *data);
size_t *Size_t_Wrap(int data);
size_t Size_t_Unwrap(size_t *data);



#ifdef SIZE_T_IMPLEMENTATION



void Size_t_Print(size_t *data) {
	printf("%zu",*data);
}



void Size_t_Free(void *data) {
	free(data);	
}



size_t *Size_t_Wrap(int data) {
	size_t *result=malloc(sizeof(*result));
	*result=data;
	return result;	
}



size_t Size_t_Unwrap(size_t *data) {
	return *data;	
}



void Size_t_Append(size_t ***sts,size_t *nsts,size_t *data) {
  (*sts)=realloc(*sts,sizeof(*sts)*((*nsts)+1));
  (*sts)[(*nsts)++]=data;
}

#endif /* SIZE_T_IMPLEMENTATION */

#endif /* SIZE_T_H */
