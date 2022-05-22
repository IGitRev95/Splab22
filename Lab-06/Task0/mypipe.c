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

int main(int argc, char **argv){
	int pipefd[2];
	pid_t cpid;
	char buf[6];
	int ret_val;

	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) {    /* Child reads from pipe */
		close(pipefd[0]);
		fprintf(pipefd[1],"hello\n");
		close(pipefd[1]);          /* Close unused write end */
		exit(EXIT_SUCCESS);
	} else {            /* Parent writes argv[1] to pipe */
		waitpid(cpid,&ret_val,0);		/* Wait for child */
		close(pipefd[1]);          /* Close unused read end */
		while (read(pipefd[0], buf, 1) > 0)
			printf("%s",buf);
		printf("\n");
		close(pipefd[0]);          /* Reader will see EOF */               
		exit(EXIT_SUCCESS);
	}
}
