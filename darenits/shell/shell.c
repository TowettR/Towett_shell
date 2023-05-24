#include "shell.h"

/**
 * main - function that takes commands
 */
int main(void)
{
	const char *cmd = "$$";
	char *buf = NULL;
	pid_t thepid;
	size_t n = 0;
	ssize_t getl;
	int m;
	int status;

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
		thepid = fork();
		if (thepid == -1)
		{
			perror("Error;le forking");
			free(buf);
			exit(1);
		}			
		else if (thepid == 0)
		{
			m = execve(buf, (char *const[]) {buf, NULL}, NULL);
				if (m == -1)
				{
					perror("Error;execution failed");
					free(buf);
					exit(1);
				}
		}
		else
		waitpid(thepid, &status, 0);
	}
	free(buf);
	return (0);
}


int args(char *argv[], int argc)
{
       if (argc < 2)
       {
	       perror("Error:less arguments");
	       exit(1);
       }
       else if (argc => 2)
       {
	       char *args[] = {buf, argv[@], NULL};
	       m = execve(buf, args, NULL);
	       if (m = -1)
	       {
		       perror("Error:second execution failed");
		       free(buf);
		       exit(1);
	       }
       }
}
