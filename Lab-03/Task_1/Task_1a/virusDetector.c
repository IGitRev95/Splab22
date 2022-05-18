#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

void PrintHex(unsigned char * buffer, int length, FILE* output);
void readVirus(virus* vir, FILE* input);
void printVirus(virus* vir, FILE* output);

int main(int argc, char **argv) {
    FILE * in;
    FILE * output = stdout;
    int argc_index;
    // size_t fread_ret_length;
    long file_length_in_bytes;
    if (argc == 1){
        printf("No files provided");
        return 1;
    }
    // support of several files as input
    for(argc_index=1;argc_index<argc;argc_index++){
        in = fopen(argv[argc_index], "rb");
        // computing byte size of file
        fseek(in,0,SEEK_END);
        file_length_in_bytes = ftell(in);
        fseek(in,0,SEEK_SET);
        // run until file is finished
        while( ftell(in) < file_length_in_bytes){
            virus *vir = malloc(sizeof(virus));
            readVirus(vir,in);
            printVirus(vir,output);
            free(vir->sig);
            free(vir);
        }

        printf("\n");
        fclose(in);
    }
   

    return 0;
}

void PrintHex(unsigned char * buffer, int length, FILE* output){
    int idx;
    for (idx=0; idx<length;idx++){
        fprintf(output, "%02X ",buffer[idx]);
    }
}

void readVirus(virus* vir, FILE* input){
    // parse sigSize
    unsigned char sigSize_buff[2];
    fread(sigSize_buff,sizeof(*sigSize_buff),2,input);
    vir->SigSize=(((short)sigSize_buff[1]) << 8) | sigSize_buff[0];
    // pasre sig
    vir->sig = (unsigned char*)(malloc(sizeof(char)*(vir->SigSize)));
    fread(vir->sig,sizeof(*(vir->sig)),vir->SigSize,input);
    // parse virus name
    fread(vir->virusName,sizeof(*(vir->virusName)),16,input);
}

void printVirus(virus* vir, FILE* output){
    fprintf(output, "Virus name: %s\n", vir->virusName);
    fprintf(output, "Virus size: %d\n", vir->SigSize);
    fprintf(output, "signature:\n");
    PrintHex(vir->sig,vir->SigSize,output);
    fprintf(output, "\n\n");
}
