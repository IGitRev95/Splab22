#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


int main(int argc, char **argv)
{
    unsigned char out_buff[136];
    FILE * abc = fopen("abc","r");
    fseek(abc,0x770,SEEK_SET);
    /*fprintf(stderr,"%ld \n",ftell(abc));*/
    fread(out_buff,1,136,abc);
    for(int i=0;i<136;i++){
        printf("%02X",out_buff[i]);
    }
    printf("\n");
    fclose(abc);
    return 0;
}
/* simple method to fill not implemented yet funcitons */
