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

int main(int argc, char **argv){
  /* TODO: Test your code */

  // - begin test task 2a -
  // char arr1[] = {'H','e','y','!'};
  // char* arr2 = map(arr1, 4, censor);
  // printf("%s\n", arr2); 
  // free(arr2);
  // - end test task 2a -

  // - begin test task 2b -
  // int base_len = 5;
  // char arr1[base_len];
  // char* arr2 = map(arr1, base_len, my_get);
  // char* arr3 = map(arr2, base_len, encrypt);
  // char* arr4 = map(arr3, base_len, dprt);
  // char* arr5 = map(arr4, base_len, decrypt);
  // char* arr6 = map(arr5, base_len, cprt);
  // free(arr2);
  // free(arr3);
  // free(arr4);
  // free(arr5);
  // free(arr6);
  // - end test task 2b -

  // - begin test task 2c -
  int base_len = 5;
  char arr1[base_len];
  char* arr2 = map(arr1, base_len, my_get);
  char* arr3 = map(arr2, base_len, quit);
  char* arr4 = map(arr3, base_len, cprt);
  free(arr2);
  free(arr3);
  free(arr4);
  // - end test task 2c -

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
  return mapped_array;
}
 
