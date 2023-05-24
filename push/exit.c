include "shell.h"
/**
 * exitprompt - function that displays prompt
 * Return: $$
 */
void exitprompt(void)
{
	char *prompt = "$$";

	write(1, prompt, 2);
}
/**
 * exitexecute - function that executes command
 * @buf: pointer
 * @argv: command input
 */
void exitexecute(char *buf, char *argv[])
{
	pid_t thepid;

	if (strcmp(buf, "exit") == 0)
	{
		exit(0);
	}
	thepid = fork();
	if (thepid == -1)
	{
		perror("Error:forking");
		free(buf);
		exit(1);
	}
	else if (thepid == 0)
	{
		execve(buf, argv, environ);
		perror("Error:execution");
		free(buf);
		exit(1);
	}
	else
	{
		int status;

		waitpid(thepid, &status, 0);
	}
}
/**
 * exitparse - function that parse command
 * @buf: pointer
 * @argv: command input
 * Return: argc
 */
int exitparse(char *buf, char *argv[])
{
	int argc = 0;
	char *token;

	token = strtok(buf, " ");
	while (token != NULL && argc < MAX_ARGUMENTS)
	{
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}
	argv[argc] = NULL;
	return (argc);
}
/**
 * exitmain - function that does it
 * Return: Always 0
 */
int exitmain(void)
{
	char *cmd;
	size_t m = 0;
	ssize_t cmd_size;
	char *args[MAX_ARGUMENTS + 1];
	int argc;

	while (1)
	{
		exitprompt();
		cmd_size = getline(&cmd, &m, stdin);
		if (cmd_size == -1)
		{
			break;
		}
		if (cmd[cmd_size - 1] == '\n')
		{
			cmd[cmd_size - 1] = '\0';
		}
		argc = exitparse(cmd, args);
		if (argc > 0)
		{
			exitexecute(args[0], args);
		}
	}
	free(cmd);
	return (0);
}
