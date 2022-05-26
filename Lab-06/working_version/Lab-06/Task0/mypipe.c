#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int debug_flag = 0; 

int main(int argc, char **argv){
    FILE* input = stdin;
    FILE* output = stdout;
    FILE* err = stderr;
    cmdLine * cmd_line;
    int pipefd[2];
    int ret_val = 0, fd = -1, new_pfd = -1, i;
    pid_t c1pid = -1, c2pid = -1;
    char ls_command[] = "ls -l", tail_command[] = "tail -n3";
     
	for(i = 1; i < argc; i++){
		if(strncmp(argv[i], "-d", 2) == 0){
			debug_flag = 1;
		}
	}
    
    if(pipe(pipefd) == -1){
        perror("fork");
        _exit(1);
    }
    
    if(debug_flag) fprintf(err, "(parent_process>forking...)\n");
    c1pid = fork();
    if(debug_flag) fprintf(err, "(parent_process>created process with id: %d)\n", c1pid);
     
    
    if(c1pid == -1){
        perror("fork1");
        _exit(1);
    }    
    
     if (c1pid == 0) { 
        if(debug_flag) fprintf(err, "(child1>redirecting stdout to the write end of the pipe...)\n");
        fd = fileno(output);
        close(fd);
        new_pfd = dup(pipefd[1]);
        close(pipefd[1]);
        
        if(debug_flag) fprintf(err, "(child1>going to execute cmd: ...)\n");
        cmd_line = parseCmdLines(ls_command);        
        ret_val = execvp(cmd_line->arguments[0], cmd_line->arguments);
        if(ret_val == -1){
            char * path = cmd_line->arguments[0];
            perror("failed in"+*path);
            _exit(1);
        }	
        _exit(0);
    }
    else { 
        if(debug_flag) fprintf(err, "(parent_process>closing the write end of the pipe...)\n");
        close(pipefd[1]);
        
        if(debug_flag) fprintf(err, "(parent_process>forking...)\n");
        c2pid = fork();
        if(debug_flag) fprintf(err, "(parent_process>created process with id: %d)\n", c2pid);
        
        if(c2pid == 0){
            if(debug_flag) fprintf(err, "(child2>redirecting stdin to the read end of the pipe...)\n");
            fd = fileno(input);
            close(fd);
            new_pfd = dup(pipefd[0]);
            if(debug_flag) fprintf(err, "(parent_process>closing the read end of the pipe...)\n");
            close(pipefd[0]);
                        
            if(debug_flag) fprintf(err, "(child2>going to execute cmd: ...)\n");
            cmd_line = parseCmdLines(tail_command);        
            ret_val = execvp(cmd_line->arguments[0], cmd_line->arguments);
            if(ret_val == -1){
                char * path = cmd_line->arguments[0];
                perror("failed in"+*path);
                _exit(1);
            }	
            _exit(0);
        }
        else{
            if(debug_flag) fprintf(err, "(parent_process>waiting for child processes to terminate...)\n");
            close(pipefd[0]);
            c1pid = waitpid(c1pid, &ret_val, 0);
            c2pid = waitpid(c2pid, &ret_val, 0);
            if(debug_flag) fprintf(err, "(parent_process>exiting...)\n");
        }

        exit(0);
    }

}


