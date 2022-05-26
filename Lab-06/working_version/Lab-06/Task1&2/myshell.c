#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define USER_LINE_BUFFER_SIZE 2048
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
	cmdLine* cmd; /* the parsed command line*/
	pid_t pid; /* the process id that is
	running the command*/
	int status; /* status of the process:
	RUNNING/SUSPENDED/TERMINATED */
	struct process *next; /* next process in chain */
} process;

void execute(cmdLine *pCmdLine);
void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void pipeline(cmdLine *cmd_line);

int exit_flag = 0;
int debug_flag = 0;
process** process_list = NULL;

int main(int argc, char **argv){
	char user_line_buff[USER_LINE_BUFFER_SIZE], directory_path_buff[PATH_MAX];
	int i;
	cmdLine * cmd_lines, * cmd_line;
	printf("Starting the program\n");
	for(i = 1; i < argc; i++){
		if(strncmp(argv[i], "-d", 2) == 0){
			debug_flag = 1;
		}
	}
	while(1) {
		/* print cur dir and get user line input */
		getcwd(directory_path_buff,PATH_MAX);
		printf("\n%s:$ ",directory_path_buff); /* cwd print */
		fgets(user_line_buff,USER_LINE_BUFFER_SIZE,stdin);
		/* parse and eceute */
		cmd_lines = parseCmdLines(user_line_buff);
		cmd_line = cmd_lines;
		/*while (cmd_line != NULL)
		{
			execute(cmd_line);
			if(exit_flag){
				break;
			}
			cmd_line = cmd_line->next;
		}*/
        
        execute(cmd_line);
        if(exit_flag){
            break;
        }        
        		
		freeCmdLines(cmd_lines);
		if(exit_flag){
				return 0;
		}
	}
	return 0;
}

void execute(cmdLine *pCmdLine){
	FILE* input = stdin;
    FILE* output = stdout;
    FILE* temp_file;
    int ret_val = 0, fd = -1, new_pfd1 = -1, new_pfd0 = -1;
	pid_t pid = -1;
	char* name = pCmdLine->arguments[0];
	if(pCmdLine != NULL){
		if(strcmp(pCmdLine->arguments[0], "quit") == 0){
			if(debug_flag){
				fprintf(stderr, "PID: %d, command: quit\n", getpid());
			}
			exit_flag = 1;
			return;
		}
		
		if(pCmdLine->next != NULL){
            pipeline(pCmdLine);            
        }else{
            if(strcmp(pCmdLine->arguments[0], "cd") == 0){
                /*char * const *args = &(pCmdLine->arguments[1]);*/
                
                ret_val = chdir(pCmdLine->arguments[1]);
                if(debug_flag){
                    fprintf(stderr, "PID: %d, command: cd\n", getpid());
                }
                if(ret_val == -1){
                    char * path = pCmdLine->arguments[0];
                    perror("failed in"+*path);
                    _exit(1);
                }
                return;
            }

            if(strcmp(pCmdLine->arguments[0], "showprocs") == 0){
                /*char * const *args = &(pCmdLine->arguments[1]);*/
                printProcessList(process_list);
                if(debug_flag){
                    fprintf(stderr, "PID: %d, command: showprocs\n", getpid());
                }
                return;
            }

            /* if cmd require forking */
            pid = fork();
            /* if forked so only child will execute */
            if(pid == 0){
                
                
                
                
                
                if(strcmp(pCmdLine->arguments[0], "cat") == 0){
                    // check if inputcmd and outputcmd are not null
                    if(pCmdLine->outputRedirect != NULL){
                        temp_file = fopen(pCmdLine->outputRedirect, "a+");
                        fd = fileno(output);
                        close(fd);
                        fd = fileno(temp_file);                
                        new_pfd1 = dup(fd);                    
                        close(fd);
                    }
                    
                    if(pCmdLine->inputRedirect != NULL){
                        temp_file = fopen(pCmdLine->inputRedirect, "a+");                    
                        fd = fileno(input);                    
                        close(fd);                    
                        fd = fileno(temp_file);                     
                        new_pfd0 = dup(fd);                    
                        close(fd);                    
                    }
                    
                    ret_val = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
                    
                    if(pCmdLine->outputRedirect != NULL){
                        dup2(new_pfd1, 1);
                        close(new_pfd1);
                        output = stdout; 
                    }
                    
                    if(pCmdLine->inputRedirect != NULL){
                        dup2(new_pfd0, 0);                
                        close(new_pfd0);
                        input = stdin;
                    }
                    
                                
                
                
                }
                else{
                    /*if child -> execute*/
                    ret_val = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
                    if(ret_val == -1){
                        char * path = pCmdLine->arguments[0];
                        perror("failed in"+*path);
                        _exit(1);
                    }
                }
                exit(ret_val);
            }
            /*if parent -> wait for child execution to end*/
            if(debug_flag){
                fprintf(stderr, "PID: %d, command: %s\n", pid, name);
            }
            if(pCmdLine->blocking){
                pid = waitpid(pid, &ret_val,0);
            }
            if(ret_val == -1){
                char * path = pCmdLine->arguments[0];
                perror("failed in"+*path);
                _exit(1);
            }
        }		
	}
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
	process* procToAdd = (malloc(sizeof(process)));
	procToAdd->cmd = cmd;
	procToAdd->pid = pid;
	procToAdd->status = RUNNING;
	procToAdd->next = NULL;
	if(*process_list == NULL){
		*process_list = procToAdd;
	}
	else {
		(*process_list)->next = procToAdd;
	}
}

void printProcessList(process** process_list){
	process* temp_proc = *process_list;
	int proc_index = 0;
	printf("#\t PID\t Command\t Status(-1=Terminated, 0=Suspended, 1=Running)\n");
	while(temp_proc != NULL){
		printf("%d\t, %d\t, %s\t, %d\t", proc_index, temp_proc->pid, temp_proc->cmd->arguments[0], temp_proc->status);
		temp_proc = temp_proc->next;
		proc_index++;
	}	
}



void pipeline(cmdLine *cmd_line){
    FILE* input = stdin;
    FILE* output = stdout;
    FILE* err = stderr;
    int pipefd[2];
    int ret_val = 0, fd = -1, new_pfd = -1;
    pid_t c1pid = -1, c2pid = -1;
    
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
            cmd_line = cmd_line->next;
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
    }
}
