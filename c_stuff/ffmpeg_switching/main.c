#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* execute command for shell */
#define SHELL "/bin/sh"

int main()
{
	pid_t pid;

run_program:

	pid = fork();
	if (pid < 0) {
		return -1;
	}

	if (pid == 0) {
		/* this is the child process, execute the shell command */
		execl(SHELL, SHELL, "-c", "sh fails.sh", NULL);

		perror("execl failed");
		_exit(EXIT_FAILURE);
	} else {
		/* this is the parent process wait for the child process to complete */
		int status;
		
		if (waitpid(pid, &status, 0) != pid) {
			return -1;
		}

		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			printf("script failed, restarting...\n");
			goto run_program;
		} else {
			printf("script success, exiting...\n");
			return status;
		}
	}
	
	return EXIT_FAILURE;
}
