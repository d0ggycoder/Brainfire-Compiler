#include <stddef.h>
typedef struct{
    int cap;
    char* data;
} Bitset;
Bitset* bitset_new(int);
int bitset_includes(Bitset*,int);
void bitset_set(Bitset*,int,int);
void bitset_free(Bitset*);

// Vectors
// Argument counting
#define __NARGS_IMPL__(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10, N, ...) N
#define __NARGS__(...) __NARGS_IMPL__(__VA_ARGS__, \
    10,9,8,7,6,5,4,3,2,1,0)
#define EXPAND(x) x
#define __VECTOR_CHOOSER__(count) _vector_new_##count
#define VECTOR_CHOOSER(count) EXPAND(__VECTOR_CHOOSER__(count))

#define vector_new(type, ...) VECTOR_CHOOSER(__NARGS__(__VA_ARGS__))(sizeof(type), ##__VA_ARGS__)

#define vector_get(v,i,t) *(t*)_vector_get(v,i)
#define vector_push(v,i) _vector_push(v,(void*)&i)
typedef struct{
    size_t element_size;
    size_t capacity;
    size_t length;
    void* data;
} Vector;
Vector* _vector_new_0(size_t);              // Constructor with element size
Vector* _vector_new_1(size_t,size_t);       // Constructor with element size and initial capacity 
Vector* _vector_new_2(size_t,size_t,void*); // Constructor with initializer list
void* _vector_get(Vector*,int);
void _vector_push(Vector*,void*);
void _vector_resize(Vector*,int);

