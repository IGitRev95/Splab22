#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum encodeMode {
    def,
    addition,
    substruction
}; 

enum bool {
    true,
    false
};

typedef struct Configs
{
    enum bool validConfig;
    enum encodeMode encMod;
    enum bool debugMod;
    int convertedCounter;
    int encryptionKey;
}Config;

int hexCharToInt(char ch){
    if (('0' <= ch) && (ch <= '9')){
        return ch - 48;
    }
    if (('A' <= ch) && (ch <= 'F')){
        return ch - 55;
    }
    printf("hexa key is out of range - %c\n", ch);
    return -1;
}


Config parseInput(int argc, char **argv){

    enum bool validConfig = true;
    enum encodeMode encMod = def;
    enum bool debugMod = false;
    int convertedCounter = 0;
    int encryptionKey = 0;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-D") == 0){
            debugMod = true;
            continue;
        }
        if(strncmp(argv[i], "+e", 2) == 0){
            int amount = hexCharToInt(argv[i][2]);
            if (amount == -1){
                validConfig = false;
                break;
            }
            encMod = addition;
            encryptionKey = amount;
            continue;
        }
        if(strncmp(argv[i], "-e", 2) == 0){
            int amount = hexCharToInt(argv[i][2]);
            if (amount == -1){
                validConfig = false;
                break;
            }
            encMod = substruction;
            encryptionKey = amount;
            continue;
        }

        // More run options ----

        printf("invalid parameter - %s\n", argv[i]);
	    validConfig = false;
        break;
    }
    
    Config cong = {validConfig,encMod,debugMod,convertedCounter,encryptionKey};
    return cong;
}


void defaultEncode(Config cong){
    FILE * in = stdin;
    FILE * out = stdout;

    int chIn,chOut;
    while( (chIn = fgetc(in)) != EOF ){
        // encoding
        if ( chIn >= 65 && chIn <= 90 ){
            chOut = 46;
        }else{
            chOut = chIn;
        }

        if(cong.debugMod == true){
            if(chOut != chIn){
                    cong.convertedCounter++;
            }
            // in debug uppon ENTER key print required text
            if(chIn == 10){ // new line ascii is 10
                fprintf(stderr,"the number of letters: %d \n", cong.convertedCounter);
                cong.convertedCounter = 0;
            }else{
                fprintf(stderr,"%d %d \n", chIn, chOut);
            }
        }
        fputc(chOut, out);
    }
}

void additionEncode(Config cong){
    FILE * in = stdin;
    FILE * out = stdout;

    int key = -1;
    int chIn;
    while( (chIn = fgetc(in)) != EOF ){
        // encoding
        if(key == -1){
            key = chIn;
        }
        if(chIn == 10){
            for(int i=0;i<cong.encryptionKey;i++){
                fputc(key, out);
            }
            fputc(chIn, out);
            key = -1;
        }else{
            fputc(chIn, out);
        }
    }
}

void substructionEncode(Config cong){
    FILE * in = stdin;
    FILE * out = stdout;
    int ignoreCounter = 0;
    int chIn;
    while( (chIn = fgetc(in)) != EOF ){
        // encoding      
        
        if(chIn != 10){
            if(ignoreCounter<cong.encryptionKey){
                ignoreCounter++;
            }else{
                fputc(chIn, out);
            }
        }else{
            fputc(chIn, out);
            ignoreCounter = 0;
        }
    }
}

int main(int argc, char **argv) {
    // FILE * in = stdin;
    // FILE * out = stdout;
    
    Config cong = parseInput(argc,argv);
    if (cong.validConfig == false){
        return 1;
    }

    switch (cong.encMod)
    {
    case def:
        defaultEncode(cong);
        break;
    case addition:
        //
        additionEncode(cong);
        break;
    case substruction:
        //
        substructionEncode(cong);
        break;
    }

    return 0;
}
