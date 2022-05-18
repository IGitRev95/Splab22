#include <stdio.h>

int main(int argc, char **argv) {
    FILE * in = stdin;
    FILE * out = stdout;

    int ch;

    while( (ch = fgetc(in)) != EOF ){

        if ( ch >= 65 && ch <= 90 ){
            fputc(46,out);
        }
        else{
            fputc(ch, out);
        }

    }

    return 0;
}
