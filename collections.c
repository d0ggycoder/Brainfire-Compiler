#include <stdlib.h>
#include <string.h>

#define __VECTOR_CHOOSER__(count) _vector_new_##count
typedef struct{
    int cap;
    char* data;
} Bitset;
Bitset* bitset_new(int capacity){
    Bitset* b = (Bitset*)malloc(sizeof(Bitset));
    b->cap = capacity;
    b->data = (char*)malloc(sizeof(char)*(capacity>>3));
    return b;
}
int bitset_includes(Bitset* b, int ind){
    return ind<b->cap && b->data[ind>>3]&(1<<(ind&0x8));
}
void bitset_set(Bitset* b, int ind, int val){
    /*
    If 
    */
    if(ind>b->cap) return;
    if(val){
        b->data[ind>>3] |= (1<<(ind&0x8));
    } else {
        b->data[ind>>3] &= ~(1<<(ind&0x8));
    }
}
void bitset_free(Bitset* b){
    free(b->data);
    free(b);
}

typedef struct{
    size_t element_size;
    size_t capacity;
    size_t length;
    void* data;
} Vector;
Vector* _vector_new_0(size_t size){
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->element_size = size;
    v->capacity = 1;
    v->length = 0;
    v->data = malloc(v->capacity);
}
Vector* _vector_new_1(size_t size,size_t capacity){
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->element_size = size;
    v->capacity = size*capacity;
    v->length = 0;
    v->data = malloc(v->capacity);
}
Vector* _vector_new_2(size_t size,size_t capacity,void* init){
    Vector* v =(Vector*)malloc(sizeof(Vector));
    v->element_size = size;
    v->capacity = capacity*size;
    v->length = capacity;
    v->data = malloc(capacity);
    memcpy(v->data,init,v->capacity);
}
void* _vector_get(Vector* v,int ind){
    return v->data+ind*v->element_size;
}
void _vector_resize(Vector* v,size_t new_cap){
    v->capacity = new_cap;
    v->data = realloc(v->data,new_cap);
}
void _vector_push(Vector* v,void* el){
    if(v->length == v->capacity){
        _vector_resize(v,v->capacity*2);
    }
    memcpy(el,v->data+v->length*v->element_size,v->element_size);
    v->length++;
}