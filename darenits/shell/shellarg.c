#include "shell.h"

/**
 * main - function that takes commands
 */
void tokens(char *stdin, char *argv[], int *argc)
{
	char *token = strtok(stdin, " ");
	int i = 0;

	while (token != NULL)
	{
		argv[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	argv[i] = NULL;
	*argc = i;
}

int main(void)
{
        const char *cmd = "$$";
        char *buf = NULL;
        pid_t thepid;
        size_t n = 0;
        ssize_t getl;
        int m;
        int status;
	char *args[100];
	char *argv[100];
        int argc;
	
	while (1)
	{
                write(1, cmd, 2);

                getl = getline(&buf, &n, stdin);
                if (getl == -1)
                {
                        perror("Error:getline error");
                        free(buf);
                        exit(1);
                }
                if (buf[getl - 1] == '\n')
                {
                        buf[getl - 1] = '\0';
                }
		if (strcmp(argv[0], "exit") == 0)
		{
			free(buf);
			exit(0);
		}
                thepid = fork();
                if (thepid == -1)
                {
                        perror("Error;le forking");
                        free(buf);
                        exit(1);
                }
                else if (thepid == 0)
		{
				 if (argc < 2)
				 {
					 perror("Error:less arguments");
					 exit(1);
				 }
				 else if (argc > 1)
				 {
					 m = execve(argv[0], argv, NULL);
					 if (m = -1)
					 {
						 perror("Error:second execution failed");
						 free(buf);
						 exit(1);
					 }
				 }
		}
                else
                waitpid(thepid, &status, 0);
        }
        free(buf);
        return (0);
}
