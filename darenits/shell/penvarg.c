#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGUMENTS 10

extern char** environ; 

void print_prompt()
{
	char *prompt = "$$";
	write(1, prompt, 2);
}

void execute_command(char* buf, char* argv[]) {
    pid_t thepid;
    thepid = fork();

    if (thepid == -1) {
        perror("Error:forking");
        exit(1);
    } 
    else if (thepid == 0) 
    {
        execve(buf, argv, environ);
        perror("Error:execution");
        exit(1);
    } 
    else 
    {
        int status;
        waitpid(thepid, &status, 0);
    }
}

int parse_command(char* buf, char* argv[]) {
    int argc = 0;

    char* token = strtok(buf, " ");
    while (token != NULL && argc < MAX_ARGUMENTS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;

    return argc;
}

int main() {
    char* buf = NULL;
    size_t n = 0;
    ssize_t getl;
    char* argv[MAX_ARGUMENTS + 1];
    
    while (1) {
        char *prompt = "$$";
	write(1, prompt, 2);

        getl = getline(&buf, &n, stdin);
        if (getl == -1)
		break;


        if (buf[getl - 1] == '\n') 
	{
            buf[getl - 1] = '\0';
        }

        int argc = parse_command(buf, argv);

        if (argc > 0) {
            execute_command(argv[0], argv);
        }
    }

    free(buf);

    return 0;
}
