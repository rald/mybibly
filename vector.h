#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define Vector_Length(v) (((Vector*)v)->length)


typedef struct Vector Vector;

typedef void Vector_PrintDataCallback(int num,void *data);
typedef void Vector_FreeDataCallback(void *data);

struct Vector {
  void **data;
  size_t capacity;
  size_t length;
  Vector_PrintDataCallback *print; 
  Vector_FreeDataCallback *Free; 
}; 

Vector *Vector_New(size_t capacity,Vector_PrintDataCallback *print,Vector_FreeDataCallback *Free);
void Vector_Free(void *vector);
void Vector_Set(Vector *vector,size_t index,void *value);
void *Vector_Get(Vector *vector,size_t index);
void Vector_Append(Vector *vector,void *value);
void Vector_Prepend(Vector *vector,void *value);
void Vector_Insert(Vector *vector,size_t index,void *value);
void Vector_Remove(Vector *vector,size_t index);
void Vector_Print(int num,Vector *vector);
void Vector_FreeData(void *vector);
void Vector_PrintData(void *vector);



#ifdef VECTOR_IMPLEMENTATION



Vector *Vector_New(size_t capacity,Vector_PrintDataCallback *print,Vector_FreeDataCallback *Free) {
  Vector *vector=malloc(sizeof(*vector));
  if(vector!=NULL) {
    vector->data=malloc(sizeof(*(vector->data))*capacity);
    vector->capacity=capacity;
    vector->length=0;
    vector->print=print;
    vector->Free=Free;
  }
  return vector;
}

void Vector_Free(void *vector) {

  for(size_t i=0;i<((Vector*)vector)->length;i++) {
    ((Vector*)vector)->Free(((Vector*)vector)->data[i]);
  }
  free(((Vector*)vector)->data);
  free(vector);
}

void Vector_Set(Vector *vector,size_t index,void *value) {
  vector->Free(vector->data[index]);
  vector->data[index]=value;
}

void *Vector_Get(Vector *vector,size_t index) {
  return vector->data[index];
}

void Vector_Append(Vector *vector,void *value) {
  if(vector->capacity==vector->length) {
    vector->capacity++;
    vector->data=realloc(vector->data,sizeof(vector->data)*(vector->capacity));
  }
  vector->data[vector->length++]=value;
}

void Vector_Prepend(Vector *vector,void *value) {
  if(vector->capacity==vector->length) {
    vector->capacity++;
    vector->data=realloc(vector->data,sizeof(vector->data)*(vector->capacity));
  }
  for(size_t i=vector->length;i>0;i--) {
    vector->data[i]=vector->data[i-1];
  }
  vector->data[0]=value;
  vector->length++;
}

void Vector_Insert(Vector *vector,size_t index,void *value) {
  assert(vector!=NULL);
  if(vector->capacity==vector->length) {
    vector->capacity++;
    vector->data=realloc(vector->data,sizeof(*(vector->data))*vector->capacity);
  }
  vector->length++;
  for(size_t i=vector->length-1;i>index;i--) {
    vector->data[i]=vector->data[i-1];
  }
  vector->data[index]=value;
}

void Vector_Remove(Vector *vector,size_t index) {
  vector->Free(vector->data[index]);
  for(size_t i=index;i<vector->length-1;i++) {
    vector->data[i]=vector->data[i+1];
  }
  vector->length--;  
}

void Vector_Print(int num,Vector *vector) {
  for(size_t i=0;i<vector->length;i++) {
    vector->print(i+1,vector->data[i]);
  }
}

void Vector_PrintData(void *vector) {
  for(size_t i=0;i<((Vector*)vector)->length;i++) {
    ((Vector*)vector)->print(i+1,((Vector*)vector)->data[i]);
  }
  printf("\n");
}

void Vector_FreeData(void *vector) {
  for(size_t i=0;i<((Vector*)vector)->length;i++) {
    ((Vector*)vector)->Free(((Vector*)vector)->data[i]);
    ((Vector*)vector)->data[i]=NULL;
  }
  free(((Vector*)vector)->data);
  free(vector);
}



#endif /* VECTOR_IMPLEMENTATION */



#endif /* VECTOR_H  */
