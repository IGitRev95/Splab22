#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char quit(char c);
char encrypt(char c);
char decrypt(char c);
char dprt(char c);
char cprt(char c);
char my_get(char c);
char censor(char c);
char* map(char *array, int array_length, char (*f) (char));

typedef struct fun_desc {
  char *name;
  char (*fun)(char);
}FuncDesc;

FuncDesc funcs[] = {
  {"Censor",censor},
  {"Encrypt",encrypt},
  {"Decrypt", decrypt},
  {"Print dec",dprt},
  {"Print string", cprt},
  {"Get string", my_get},
  {"Quit", quit},
  {NULL,NULL}
};

int main(int argc, char **argv){
  /* TODO: Test your code */
  int menuBound=0, option; 
  // char carry[5]= "";
  char * carry = (char*)(malloc(5*sizeof(char)));
  size_t i;
  // compute func bound
  for( i=0; funcs[i].name!=NULL ; i++){
    menuBound++;
  }
  // main loop
  while (1)
  {
    printf("Please choose a function:\n");
    for( i=0; funcs[i].name!=NULL ; i++){
      printf("%d) %s\n",i,funcs[i].name);
    }
    printf("Option: ");
    option = fgetc(stdin);
    fgetc(stdin); // emitt '\n' of ENTER
    
    option = option-48;
    if(!( option >= 0 && option<menuBound)){
      printf("Not within bounds");
      return 0;
    }
    printf("Within bounds\n");
    
    carry = map(carry, 5, funcs[option].fun);
    printf("DONE.\n");
  }
    
  return 0;
}



// Task 2c
/* Gets a char c,  and if the char is 'q' ,
    ends the program with exit code 0. Otherwise returns c. */
char quit(char c){
  if ( c == 'q'){
    exit(0);
  }
  return c;
}

// Task 2b
/* Gets a char c and returns its encrypted form by adding 2 to its value. 
    If c is not between 0x41(65) and 0x7a(122) it is returned unchanged */
char encrypt(char c){
  if( c < 0x41 || c > 0x7a ){
    return c;
  }else{
    return c+2;
  }
}

/* Gets a char c and returns its decrypted form by reducing 2 to its value. 
    If c is not between 0x41 and 0x7a it is returned unchanged */
char decrypt(char c){
  if( c < 0x41 || c > 0x7a ){
    return c;
  }else{
    return c-2;
  }
}

/* dprt prints the value of c in a decimal representation followed by a 
    new line, and returns c unchanged. */
char dprt(char c){
  printf("%d\n",c);
  return c;
}

/* If c is a number between 0x41 and 0x7a, cprt prints the character of ASCII value c followed 
    by a new line. Otherwise, cprt prints the dot ('*') character. After printing, cprt returns 
    the value of c unchanged. */
char cprt(char c){
  if( c < 0x41 || c > 0x7a ){
    printf("*\n");
  }else{
    printf("%c\n",c);
  }
  return c;
}

/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c){
  return fgetc(stdin);
}

// Task 2a
char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  int i;
  /* task 2.a */
  for( i=0; i<array_length; i++){
    mapped_array[i] = f(array[i]);
  }
  free(array);
  return mapped_array;
}
 
