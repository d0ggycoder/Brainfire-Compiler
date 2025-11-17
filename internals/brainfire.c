#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CALLDEPTH 512
typedef struct{
    char* value;
    size_t len;
} str;
const str INSTRUCTION_SET[] = {
    {"call shift_r\n",13},              //rshift
    {"call shift_l\n",13},              //lshift
    {"addb $1,(%rax)\n",15},            //inc
    {"subb $1,(%rax)\n",15},            //dec
    {"call p_byte\n",16},               //print byte
    {"call p_char\n",16},               //print char
    {"loop%d:\n",0},                    //start loop
    {"cmpb $0,(%rax)\njnz loop%d\n",0}  //end loop
};

int file_copy(const char* src,const char* dest){
    char* errorMessage;
    FILE *fin = fopen(src,"r");
    if(fin == NULL){
        errorMessage = (char*)malloc(sizeof(char) * (21+strlen(src)));//len(Error opening file: ) = 20+1
        sprintf(errorMessage,"Error opening file: %s",src);
        perror(errorMessage);
        return EXIT_FAILURE;
    } 
    
    FILE *fout = fopen(dest,"w");
    if(fout == NULL){
        errorMessage = (char*)malloc(sizeof(char) * (21+strlen(dest)));//len(Error opening file: ) = 20+1
        sprintf(errorMessage,"Error opening file: %s",dest);
        perror(errorMessage);
        return EXIT_FAILURE;
    } 

    char rbuf[1024];
    size_t bytes_read;
    while((bytes_read = fread(rbuf, 1, sizeof(rbuf)-1, fin)) > 0 ){
        if(fwrite(rbuf, sizeof(char), bytes_read, fout)<bytes_read){
            errorMessage = (char*)malloc(sizeof(char) * (25+strlen(dest)));//len(Error writing to file: ) = 24+1
            sprintf(errorMessage,"Error opening file: %s",dest);
            return EXIT_FAILURE;
        }
    }
    fclose(fin);
    fclose(fout);
    return EXIT_SUCCESS;
}
int asm_compile(char* baseFile){
    char command[256];
    snprintf(command, sizeof(command), "gcc -nostdlib %s.s -o %s",baseFile,baseFile); 
    if(system(command) != 0){
        perror("Error compiling");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int compile(char* src, char* dst){
    char* errorMessage;
    FILE *fin = fopen(src,"r");
    if(fin == NULL){
        errorMessage = (char*)malloc(sizeof(char) * (21+strlen(src)));//len(Error opening file: ) = 20+1
        sprintf(errorMessage,"Error opening file: %s",src);
        printf("%s",errorMessage);
        return EXIT_FAILURE;
    } 

    FILE *fout = fopen(dst,"a");
    if(fout == NULL){
        errorMessage = (char*)malloc(sizeof(char) * (21+strlen(dst)));//len(Error opening file: ) = 20
        sprintf(errorMessage,"Error opening file: %s",dst);
        printf("%s",errorMessage);
        return EXIT_FAILURE;
    }
    char rbuf[1];
    char inst[256];
    char instruction;
    int callstack[CALLDEPTH];
    int callind = 0;
    int callid = 0;
    int len = 0;
    //TODO: Add /comment/ capability and print byte vs. print char
    while(fread(rbuf,sizeof(char),1,fin) != 0){
        instruction = *rbuf;
        switch(instruction){
            case '>':
                fwrite(INSTRUCTION_SET[0].value,sizeof(char),INSTRUCTION_SET[0].len,fout);
                break;
            case '<':
                fwrite(INSTRUCTION_SET[1].value,sizeof(char),INSTRUCTION_SET[1].len,fout);
                break;
            case '+':
                fwrite(INSTRUCTION_SET[2].value,sizeof(char),INSTRUCTION_SET[2].len,fout);
                break;
            case '-':
                fwrite(INSTRUCTION_SET[3].value,sizeof(char),INSTRUCTION_SET[3].len,fout);
                break;
            case '.':
                fwrite(INSTRUCTION_SET[4].value,sizeof(char),INSTRUCTION_SET[4].len,fout);
                break;
            case '(':
                if(callind==CALLDEPTH-1){
                    printf("Error: Exceeded maximum loop depth\n");
                    return EXIT_FAILURE;
                }
                len = sprintf(inst,INSTRUCTION_SET[5].value,callid);
                fwrite(inst,sizeof(char),len,fout);
                callstack[callind++] = callid++;
                break;
            case ')':
                if(callind == 0){
                    printf("Error: Extraneous loop end\n");
                    return EXIT_FAILURE;
                }
                len = sprintf(inst,INSTRUCTION_SET[6].value,callstack[--callind]);
                fwrite(inst,sizeof(char),len,fout);
                break;
            default:
                printf("Error: unknown instruction%c\n",instruction);
                return EXIT_FAILURE;
        }
    }
    fwrite("ret\n",sizeof(char),4,fout); 
    fclose(fin);
    fclose(fout);
    return EXIT_SUCCESS;
}
int main(int argc, char* argv[]){
    // const char *out = "test.txt";
    // const char *message = "Hello, world\n";
    // FILE *fp = fopen(out, "w");
    // if(fp == NULL){
    //     perror("Error opening file");
    //     return EXIT_FAILURE;
    // } 
    // size_t len = fwrite(message, sizeof(char), strlen(message),fp);
    // if(len!=strlen(message)){
    //     perror("Error writing to file");
    //     return EXIT_FAILURE;
    // }

    // if(fclose(fp) != 0){
    //     perror("Error closing file");
    //     return EXIT_FAILURE;
    // }

    // printf("Wow, it worked!");
    // return EXIT_SUCCESS;
    
    if(argc != 2){
        printf("Error, wrong number of arguments provided");
        return EXIT_FAILURE;
    }
    char* orig = (char*)malloc(sizeof(char)*strlen(argv[1]));
    strcpy(orig, argv[1]);

    char* baseFile = strtok(argv[1],".");
    char* fileExt =  strtok(NULL, ".");
    if(fileExt == NULL || strcmp(fileExt,"bf") != 0){
        printf("Error, wrong file format (expected .bf)");
        return EXIT_FAILURE;
    }

    const char* cpy_src = "internals/brainfire_template.s";
    char cpy_dst[256];
    snprintf(cpy_dst,256,"%s.s",baseFile);
    if(file_copy(cpy_src,cpy_dst) == EXIT_FAILURE) return EXIT_FAILURE;
    if(compile(orig,cpy_dst) == EXIT_FAILURE) return EXIT_FAILURE;
    if(asm_compile(baseFile) == EXIT_FAILURE) return EXIT_FAILURE;
    return 0;
}