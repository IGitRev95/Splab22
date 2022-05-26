#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv){
    /* FILE* input = stdin; */
    FILE* output = stdout;
    int pipefd[2];
    int ret_val = 0;
    pid_t cpid = -1;
    char buff[] = "hello"; 
    
        
    if(pipe(pipefd) == -1){
        perror("fork");
        _exit(1);
    }
    
    cpid = fork();
    
    if(cpid == -1){
        perror("fork");
        _exit(1);
    }
    
    if (cpid == 0) {    /* Child reads from pipe */
        close(pipefd[0]); /* Close unused read end */
        write(pipefd[1], &buff, strlen(buff));
        close(pipefd[1]);
        _exit(0);

    } else {  /* Parent writes argv[1] to pipe */
        cpid = waitpid(cpid, &ret_val, 0);
        close(pipefd[1]);   /* Close unused write end */
        read(pipefd[0], buff, 6);
        printf("%s\n", buff);
        close(pipefd[0]);          /* Reader will see EOF */
          /* Wait for child */
        exit(0);
    }

}
