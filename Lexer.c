#include <stdio.h>
#include "collections.h"

typedef enum {
    IDENTIFIER,
    KEYWORD,
    PUNCTUATOR,
    OPERATOR,
    LITERAL,
} TOKENTYPE;
typedef struct{
    char* content;
    TOKENTYPE type;
} Token;

Bitset* genCharset(const char* chars){
    __NARGS__();
    Bitset* a = bitset_new(128);
    for(int i=0;chars[i];i++) bitset_set(a,chars[i],1);
    return a;
}
Token tokenize(const char* input){
    /*
    How do we tokenize a string quickly???

    */

    for(int i=0;input[i];i++){

    }
}
int main(int argc, char* argv[]){
    Bitset* punctuation_set = getCharset("(){} \t\n;"); 
    return 0;
}