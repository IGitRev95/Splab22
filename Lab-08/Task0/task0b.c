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
#define EI_DATA 5

struct fun_desc {
    char *name;
    void (*fun)(void);
};

void stubs();
void debug();
void examine();
void print_section_names();
void print_symbols();
void quit();

int debug_flag = 0;
int Currentfd = -2; /* -2 just for initial value generally defeckt will be indicated by -1 */
void* map_start;


int main(int argc, char **argv){
    int i, choise_index;
    char choise_index_str[6];
    struct fun_desc menu[] = { { "Toggle Debug Mode", debug },
                               { "Examine ELF File", examine },
                               { "Print Section Names", print_section_names },
                               { "Print Symbols", print_symbols }, 
                               { "Quit", quit }, 
                               { NULL, NULL } }; 

    while(1){
        printf("Choose action:\n");

        for(i=0; menu[i].name!=NULL ;i++){
            printf("%d-%s\n",i,menu[i].name);
        }

        fgets(choise_index_str,i,stdin);
        choise_index = atoi(choise_index_str);

        menu[choise_index].fun();
    }


    return 0;
}
/* simple method to fill not implemented yet funcitons */
void stubs(){
    printf("Not implemented yet!");
}

void debug(){
   debug_flag = !debug_flag; 
   if(debug_flag){
    printf("Debug flag is now off\n");
   }
   else{
    printf("Debug flag is now on\n");
   }
   debug_flag = 1 - debug_flag;   
}

void examine(){
    int buff_index = 0;     
    long file_size;
    char file_name[32], c;
    printf("Enter ELF file name:\n");
    while (((c = fgetc(stdin)) != '\n') && buff_index <31 )
    {
        file_name[buff_index] = c;
        buff_index++;
    }
    file_name[buff_index] = '\0';

    if(Currentfd >= 3){  /* 0-2 is the default file descriptors */
        close(Currentfd);
    }
    
    Currentfd = open(file_name, O_RDWR);
    file_size = lseek(Currentfd,0,SEEK_END);
    lseek(Currentfd,0,SEEK_SET);

    map_start = mmap(NULL,file_size,PROT_READ,MAP_SHARED,Currentfd,0); /* PORT_READ is the memory protection like fopen with 'r' flag */  

    printf("map_start: %x", (int)map_start);
    printf("map_start: %c", *(int *)map_start);

    Elf32_Ehdr *elf_head = (Elf32_Ehdr*)map_start;
    unsigned char *elf_ident = elf_head->e_ident;
    int ascii_magic_num[] = {127, 69, 76, 70};
    int i, elf_file_flag = 1;
    for(i = 0; i < 4; i++){
        if(!(ascii_magic_num[i] == elf_ident[i])){
            elf_file_flag = 0;
        }
        printf("%c ", ascii_magic_num[i]);
    }
    printf("\n");
    if(!elf_file_flag){
        printf("Not an elf file...");
        quit();
    }

    printf("Data encoding scheme: ");
    switch (elf_ident[EI_DATA]){
        case 0:
            printf("ELFDATANONE\n");
            break;
        
        case 1:
            printf("ELFDATA2LSB\n");
            break;
            
        case 2:
            printf("ELFDATA2MSB\n");
            break;
    }
    
    printf("Entry point: 0x%08x\n", elf_head->e_entry);

    printf("Offset to section header table: 0x%02x\n", elf_head->e_shoff);

    printf("Number of sections: %d\n", elf_head->e_shnum);



}

void print_section_names(){
    stubs();

    
}

void print_symbols(){
    stubs();
}

void quit(){
    stubs();
    exit(0);
}