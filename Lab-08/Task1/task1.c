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
void global_close();

int debug_flag = 0;
int Currentfd = -2; /* -2 just for initial value generally defeckt will be indicated by -1 */
void *curr_addr = NULL;
size_t curr_file_size;
char curr_file_name[32];

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
    printf("Not implemented yet!\n");
}

void debug(){
   if(debug_flag){
    printf("Debug flag is now off\n");
   }
   else{
    printf("Debug flag is now on\n");
   }
   debug_flag = 1 - debug_flag;   
}

void examine(){
    int buff_index = 0, i, elf_file_flag;     
    char c;
    Elf32_Ehdr *elf_head;
    unsigned char *elf_ident;
    int ascii_magic_num[] = {127, 69, 76, 70};
    char* encoding[] = {"Little Endian\0", "Big Endian\0", "Invalid\0"};
    
    printf("Enter ELF file name:\n");
    while (((c = fgetc(stdin)) != '\n') && buff_index <31 )
    {
        curr_file_name[buff_index] = c;
        buff_index++;
    }
    curr_file_name[buff_index] = '\0';

    if(Currentfd >= 3){  /* 0-2 is the default file descriptors */
        global_close();
    }
    
    Currentfd = open(curr_file_name, O_RDWR);
    curr_file_size = lseek(Currentfd,0,SEEK_END);
    lseek(Currentfd,0,SEEK_SET);

    if(debug_flag){
        
    }
    
    map_start = mmap(curr_addr,curr_file_size,PROT_READ,MAP_SHARED,Currentfd,0); /* PORT_READ is the memory protection like fopen with 'r' flag */  

    if(debug_flag){
        printf("\t-----Debug info-----\nCurrent fd: %d, file size: %d, map_start: %p\n\t--------------------\n", Currentfd, curr_file_size, map_start);
    }
    
    elf_head = (Elf32_Ehdr*)map_start;
    elf_ident = elf_head->e_ident;
    elf_file_flag = 1;
    
    for(i = 0; i < 4; i++){
        if(ascii_magic_num[i] != elf_ident[i]){
            elf_file_flag = 0;
        }
    }
    
    if(!elf_file_flag){
        printf("Not an elf file...\n");
        quit();
    }

    switch (elf_ident[EI_DATA]){     
        case ELFDATA2LSB:
            i = 0;
            break;
            
        case ELFDATA2MSB:
            i = 1;
            break;
            
        default:
            i = 2;
            break; 
    }
    
    printf("1. Bytes 1,2,3 of the magic number: %c, %c, %c\n", elf_ident[EI_MAG1], elf_ident[EI_MAG2], elf_ident[EI_MAG3]);
    printf("2. Data encoding scheme: %s\n", encoding[i]);
    printf("3. Entry point: 0x%08x\n", elf_head->e_entry);
    printf("4. Offset to section header table: 0x%02x\n", elf_head->e_shoff);
    printf("5. Number of sections: %d\n", elf_head->e_shnum);
    printf("6. Size of each section header entry: %d\n", elf_head->e_shentsize);
    printf("7.The file offset in which the program header table resides: %d\n", elf_head->e_phoff);
    printf("8.The number of program header entries: %d\n", elf_head->e_phnum);
    printf("9.The size of each program header entry: %d\n", elf_head->e_phentsize);
    
    printf("\n");


}

void print_section_names(){
    unsigned char *headerTable;
    int i;
    unsigned int type_of_section;
    Elf32_Ehdr *elf_head;
    Elf32_Shdr *section_header;
    
    if (Currentfd < 3){
        printf("No file selected...\n");
        return;
    }
        
    elf_head = (Elf32_Ehdr*)map_start;
    /*printf("elf_head: %p\n",  elf_head);*/
    section_header = (Elf32_Shdr*)(map_start + elf_head->e_shoff);
    /*printf("section_header: %p\n",  section_header);*/
    /*printf("headerTable: %p\n", elf_head + section_header[28].sh_offset);*/
    headerTable = (unsigned char*)(map_start + section_header[elf_head->e_shstrndx].sh_offset);


    printf("%s \t %s\t\t\t %s\t %s\t %s\t %s\n", "[#]", "Name", "Address", "Offset", "Size", "Type");    
    
    for (i = 1; i < elf_head->e_shnum; i++){
        type_of_section = section_header[i].sh_type;

        printf("[%d] \t %s\t\t %x\t\t %x\t %x\t %d\n", i, headerTable + section_header[i].sh_name, section_header[i].sh_addr,
            section_header[i].sh_offset, section_header[i].sh_size, type_of_section);
        
        if (debug_flag){
            printf("\t-----Debug: %x\n", section_header[elf_head->e_shstrndx].sh_offset + section_header[i].sh_offset);
        }
    }
}

void print_symbols(){
    stubs();
}

void quit(){
    global_close();
    exit(0);
}

void global_close(){
    munmap(curr_addr, curr_file_size);
    close(Currentfd);
    curr_addr = NULL;
    curr_file_size = 0;  
    Currentfd = -2;    
}
