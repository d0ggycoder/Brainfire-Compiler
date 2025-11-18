#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define stringify(s) {s,strlen(s)}
#define CALL_DEPTH 1024
#define BUFFER_SIZE 1024

typedef struct {
    char* v;
    int length;
} str;
const str INSTRUCTION_SET[] = {
    stringify("call shift_r\n"),                //rshift
    stringify("call shift_l\n"),                //lshift
    stringify("addb $1,(%rax)\n"),              //inc
    stringify("subb $1,(%rax)\n"),              //dec
    stringify("call p_byte\n"),                 //print byte
    stringify("call p_char\n"),                 //print char
    stringify("loop%d:\n"),                     //start loop
    stringify("cmpb $0,(%%rax)\njnz loop%d\n")   //end loop
};

FILE* openSafeFile(const char* pathname,const char* mode){
    char* errorMessage;
    FILE *file = fopen(pathname,mode);
    if(file == NULL){
        errorMessage = (char*)malloc(sizeof(char) * (21+strlen(pathname)));//len(Error opening file: ) = 20+1
        sprintf(errorMessage,"Error opening file: %s",pathname);
        perror(errorMessage);
        return NULL;
    } 
    return file;
}
int compile(const char* progRef, const char* src, const char* dst){
    // Open files and verify they opened correctly
    FILE* ftemplate = openSafeFile(progRef,"r");
    FILE* fin = openSafeFile(src,"r");
    FILE* fout = openSafeFile(dst,"w");
    if(fin == NULL || fout == NULL) return EXIT_FAILURE;

    // Copy lines until code start block
    char LINEBUFFER[BUFFER_SIZE];
    while(strcmp(fgets(LINEBUFFER,1024,ftemplate),"#CODESTART\n")) fwrite(LINEBUFFER,sizeof(char),strlen(LINEBUFFER),fout);

    // Compiler variables
    char cref[1];       // Current character
    int isComment = 0;  // Handle comments
    int len;

    int callstack[CALL_DEPTH];      // Loop callstack
    int callInd = 0; int callID = 0;// Current call index and ID

    // Compiler loop
    while(fread(cref,sizeof(char),1,fin) != 0){
        if(isComment^=(cref[0]=='/')) continue;
        switch(cref[0]){
            case '>': fwrite(INSTRUCTION_SET[0].v,sizeof(char),INSTRUCTION_SET[0].length,fout); break;
            case '<': fwrite(INSTRUCTION_SET[1].v,sizeof(char),INSTRUCTION_SET[1].length,fout); break;
            case '+': fwrite(INSTRUCTION_SET[2].v,sizeof(char),INSTRUCTION_SET[2].length,fout); break;
            case '-': fwrite(INSTRUCTION_SET[3].v,sizeof(char),INSTRUCTION_SET[3].length,fout); break;
            case '^': fwrite(INSTRUCTION_SET[4].v,sizeof(char),INSTRUCTION_SET[4].length,fout); break;
            case '.': fwrite(INSTRUCTION_SET[5].v,sizeof(char),INSTRUCTION_SET[5].length,fout); break;
            case '(': 
                if(callInd == CALL_DEPTH-1) {printf("Error, exceeded maximum call depth\n"); return EXIT_FAILURE;}
                len = sprintf(LINEBUFFER,INSTRUCTION_SET[6].v,callID);
                callstack[callInd++] = callID++;
                fwrite(LINEBUFFER,sizeof(char),len,fout);
                break;
            case ')': 
                if(callInd-- <= 0) {printf("Error, close loop does not have a matching open loop\n"); return EXIT_FAILURE;}
                len = sprintf(LINEBUFFER,INSTRUCTION_SET[7].v,callstack[callInd]);
                fwrite(LINEBUFFER,sizeof(char),len,fout);
                break;
            default:
                if(cref[0] == ' ' || cref[0] == '\n' || cref[0] == '\t' || cref[0] == '\r') return EXIT_FAILURE;
                printf("Unknown instruction: %c\nTerminating compilation\n",cref[0]);
        }
    } 
    // Finish reading the rest of the origin file
    while(fgets(LINEBUFFER,1024,ftemplate) != NULL) fwrite(LINEBUFFER,sizeof(char),strlen(LINEBUFFER),fout);
    return 0;
}
int main(int argc, char* argv[]){
    /**
     * Args:
     *   0: pathname
     *   1: source path
     *   2: output path
     */

    if(argc != 3) {printf("Error: expected 2 arguments to the compiler, recieved %d\n",argc-1); return EXIT_FAILURE;}
    if(compile("internals/brainfire_runner.s",argv[1],argv[2]) == EXIT_FAILURE) return EXIT_FAILURE;
    return 0;
}