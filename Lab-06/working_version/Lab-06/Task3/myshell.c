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
void pipeLine(cmdLine *cmd_line);
void catit(cmdLine *cmd_line);

int ** createPipes(int nPipes);
void releasePipes(int **pipes, int nPipes);
int *leftPipe(int **pipes, cmdLine *pCmdLine);
int *rightPipe(int **pipes, cmdLine *pCmdLine);

int exit_flag = 0, debug_flag = 0, single_exec_flag = 0;
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
        if(strncmp(user_line_buff, "\n", 1) == 0){continue;}
        
		/* parse and eceute */
		cmd_lines = parseCmdLines(user_line_buff);
		cmd_line = cmd_lines;
        
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
    int ret_val = 0;
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
		
		if((pCmdLine->next != NULL) && (!single_exec_flag)){
            pipeLine(pCmdLine);            
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
                    catit(pCmdLine);                  
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

void catit(cmdLine *pCmdLine){
    FILE* input = stdin;
    FILE* output = stdout;
    FILE* temp_file;
    int fd = -1, new_pfd1 = -1, new_pfd0 = -1;
    
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

    execvp(pCmdLine->arguments[0], pCmdLine->arguments);

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
    return;
}


void pipeLine(cmdLine *cmd_line){
    cmdLine *local_cmdLine = cmd_line;
    int pipe_counter = 0;
    int pipefd[2];
    int **pipe_arr;
    pid_t c1pid;
    
    while(local_cmdLine->next != NULL){
        pipe_counter++;
        local_cmdLine = local_cmdLine->next;
    }
    
    pipe_arr = createPipes(pipe_counter);
    int i = 0;
    while(cmd_line != NULL){        
        
        c1pid = fork();
        if (c1pid == 0) { 
            
            if(i != 0){ // if it is not the first command
                pipefd[0] = pipe_arr[i - 1][0];
                dup2(pipefd[0], 0); // child reads from the previous command pipe read-end
                close(pipe_arr[i-1][0]); // closing previous pipe read-end
                close(pipe_arr[i-1][1]); //// closing previous pipe write-end
            }
            
            if(i < pipe_counter){ // if it is not the last command
                pipefd[1] = pipe_arr[i][1];
                close(pipe_arr[i][0]); // closing current pipe read-end
                dup2(pipefd[1], 1); // child writes to the current command (itself) write-end
                close(pipefd[1]); // closing current pipe write-end
                
            }
            
            if(strcmp(cmd_line->arguments[0], "cat") == 0){                   
                catit(cmd_line);                  
            } else {
                execvp(cmd_line->arguments[0], cmd_line->arguments); // c1pid executed whilst its output is pipe[0];
            }            
            _exit(0);
        } else {
            if(c1pid < 0){
                perror("failed in pipeLine\n");
                exit(1);
            }
            if(i != 0){
                close(pipe_arr[i-1][1]); // closing previous pipe write-end
                close(pipe_arr[i-1][0]); // closing previous pipe read-end
            }
            waitpid(c1pid, 0, 0);
        }
        cmd_line = cmd_line->next;
        i++;
    }
    releasePipes(pipe_arr, pipe_counter);
}


int ** createPipes(int nPipes){
    int** pipes;
    pipes=(int**) calloc(nPipes, sizeof(int));

    for (int i=0; i<nPipes;i++){
        pipes[i]=(int*) calloc(2, sizeof(int));
        pipe(pipes[i]);
    }  
    return pipes;

    }
void releasePipes(int **pipes, int nPipes){
        for (int i=0; i<nPipes;i++){
            free(pipes[i]);
        
        }  
    free(pipes);
}
int *leftPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->idx == 0) return NULL;
    return pipes[pCmdLine->idx -1];
}

int *rightPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->next == NULL) return NULL;
    return pipes[pCmdLine->idx];
}

