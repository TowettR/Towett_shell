#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

extern char** environ; // External variable holding the environment

void print_prompt() {
	char *prompt = "$$";
	write(1, prompt, 2);
}

void execute_command(char* buf) {
    pid_t thepid;
    thepid = fork();

    if (thepid == -1) 
    {
        perror("Error:forking");
        exit(1);
    } 
    else if (thepid == 0) 
    {
        char* argv[] = {buf, NULL};
        execve(buf, argv, environ);
        perror("Error:execution");
        exit(1);
    } else 
    {
        int status;
        waitpid(thepid, &status, 0);
    }
}

int main() 
{
    char* buf = NULL;
    size_t n;
      
    while (1) 
    {
	    char *prompt = "$$";
	    write(1, prompt, 2);
	    ssize_t getl = getline(&buf, &n, stdin);
	    if (getl == -1)
		    break;
	if (buf[(getl - 1)] == '\n')
	{
		buf[(getl - 1)] = '\0';
	}

        execute_command(buf);
}
free(buf);
return (0);
}
