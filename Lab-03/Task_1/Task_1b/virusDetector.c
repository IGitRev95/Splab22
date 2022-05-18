#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};

typedef struct fun_desc {
  char *name;
  link* (*fun)(link*);
}FuncDesc;

//task 0_b

//prints length bytes from memory location buffer, in hexadecimal format.
void PrintHex(unsigned char * buffer, int length, FILE* output);

//task 1_a

//this function receives a pointer to a virus struct and a file pointer and overwrites the given virus struct so that it represents the next virus in the file.
void readVirus(virus* vir, FILE* input);
//this function receives a virus and a pointer to an output file. The function prints the virus to the given output. It prints the virus name (in ASCII), the virus signature length (in decimal), and the virus signature (in hexadecimal representation).
void printVirus(virus* vir, FILE* output);

//task 1_b

//Print the data of every link in list to the given stream. Each item followed by a newline character.
void list_print(link *virus_list, FILE* output);
//Add the given link to the list (either at the end or the beginning, depending on what your TA tells you), and return a pointer to the list (i.e., the first link in the list). If the list is null - return the given entry.
link* list_append(link* virus_list, link* to_add);
//Free the memory allocated by the list.
void list_free(link *virus_list);

// Load signatures - functionality returns the input file stream source
link* load_sig(link * virus_list);
// Print signatures - print the sig list and returns the list
link* print_sig(link * virus_list);
// Quit - freeing memory and quiting the program
link* quit(link * virus_list);

FuncDesc funcs[] = {
  {"Load signatures",load_sig},
  {"Print signatures",print_sig},
  {"Quit", quit},
  {NULL,NULL}
};

int main(int argc, char **argv) {
    link* virus_list = NULL;
    int i, option;
    char buffer[10];

    // main loop
    while (1){
        printf("Please choose a function:\n");
        for( i=1; funcs[i-1].name!=NULL ; i++){
            printf("%d) %s\n",i,funcs[i-1].name);
        }
        printf("Option: ");
        scanf("%s", buffer);
        option = atoi(buffer);
        //reset stdin
        fflush(stdin);
        
        if(!( option > 0 && option <= 4 )){
            printf("Not within bounds");
            return -1;
        }
        printf("Within bounds\n");
        virus_list = funcs[option-1].fun(virus_list);
        printf("DONE.\n");
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
    fprintf(output, "\n");
}

void list_print(link *virus_list, FILE* output){
    if(virus_list!=NULL){
    printVirus(virus_list->vir,output);
    fprintf(output,"\n");
    list_print(virus_list->nextVirus, output);
    }
}

link* list_append(link* virus_list, link* to_add){
    // chose to link the addition to the beginning
    if(virus_list==NULL){
        virus_list=to_add;
    }else{
        to_add->nextVirus=virus_list;
        virus_list=to_add;
    }
    return virus_list;
}

void list_free(link *virus_list){
     if(virus_list!=NULL){
         list_free(virus_list->nextVirus);
         free(virus_list->vir->sig);
         free(virus_list->vir);
         free(virus_list);
     }
}

link* load_sig(link * virus_list){
    FILE* signature_fp;
    char str[30];
    long file_length_in_bytes;
    list_free(virus_list);
    virus_list = NULL;
    printf("Please enter signature file name:\n");
    // fgets(str,30,stdin);
    scanf("%s", str);
    signature_fp = fopen(str , "rb");
    if(signature_fp == NULL){
        perror("Error opening file");
        exit(-1);
    }
    // computing byte size of file
    fseek(signature_fp,0,SEEK_END);
    file_length_in_bytes = ftell(signature_fp);
    fseek(signature_fp,0,SEEK_SET);
    // run until file is finished
    while( ftell(signature_fp) < file_length_in_bytes){
        link *virus_link = malloc(sizeof(link));
        virus_link->nextVirus=NULL;
        virus_link->vir = malloc(sizeof(virus));
        readVirus(virus_link->vir,signature_fp);
        virus_list = list_append(virus_list,virus_link);
    }
    fclose(signature_fp);

    return virus_list;
}

link* print_sig(link * virus_list){
    if(virus_list != NULL){
        list_print(virus_list, stdout);
    }
    return virus_list;
}

link* quit(link * virus_list){
    list_free(virus_list);
    exit(0);
    return virus_list;
}