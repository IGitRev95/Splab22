#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void PrintHex(unsigned char * buffer, int length);


int main(int argc, char **argv) {
    FILE * in;
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
        
        long file_dex;
        unsigned char buffer[1];
        // Reading and converting bytes to hexa one by one
        for(file_dex=0;file_dex<file_length_in_bytes;file_dex++){
            fread(buffer,sizeof(*buffer),1,in);
            PrintHex(buffer,1);
        }
        printf("\n");
        fclose(in);
    }
   

    return 0;
}

void PrintHex(unsigned char * buffer, int length){
    int idx;
    for (idx=0; idx<length;idx++){
        printf("%02X ",buffer[idx]);
    }
}
