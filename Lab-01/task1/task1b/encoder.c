#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE * in = stdin;
    FILE * out = stdout;
    
    int debugMode = 0;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-D") == 0){
            debugMode = 1;
        }else{
            printf("invalid parameter - %s\n", argv[i]);
	        return 1;
        }
    }

    int convertedCounter = 0;

    int chIn,chOut;

    while( (chIn = fgetc(in)) != EOF ){

        // encoding
        if ( chIn >= 65 && chIn <= 90 ){
            chOut = 46;
        }
        else{
            chOut = chIn;
        }

        if(debugMode == 1){
            if( chOut != chIn){
                convertedCounter++;
            }

            // in debug uppon ENTER key print required text
            if(chIn == 10){ // new line ascii is 10
                fprintf(stderr,"the number of letters: %d \n", convertedCounter);
                convertedCounter = 0;
            }else{
                fprintf(stderr,"%d %d \n", chIn, chOut);
            }
        }
        
        fputc(chOut, out);
        
    }

    return 0;
}
