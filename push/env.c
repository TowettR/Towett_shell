#include "shell.h"
extern char **environ;

/**
 * print_prompt - function that displays prompt
 * Return: $$
 */

void print_prompt(void)
{
	char *prompt = "$$";

	write(1, prompt, 2);
}
/**
 * execute - function that executes commands
 * @cmd: pointer to commands
 * @args: command input by user
 * Return: output
 */
void execute(char *cmd, char *args[])
{
	pid_t thepid;

	if (strcmp(cmd, "exit") == 0)
	{
		exit(0);
	}
	else if (strcmp(cmd, "env") == 0)
	{
		char **env = environ;

		while (*env != NULL)
		{
			printf("%s\n", *env);
			env++;
		}
	}
	else
	{
		thepid = fork();
		if (thepid == -1)
		{
			execve(cmd, args, environ);
			perror("Error:execution");
			exit(1);
		}
		else
		{
			int status;

			waitpid(thepid, &status, 0);
		}
	}
}
/**
 * parse_command - function that parses commands
 * @cmd: pointer to commands
 * @args:command input by user
 * Return: argc
 */
int parse_command(char *cmd, char *args[])
{
	int argc = 0;
	char *token = strtok(cmd, " ");

	while (token != NULL && argc < MAX_ARGUMENTS)
	{
		args[argc++] = token;

		token = strtok(NULL, " ");
	}
	args[argc] = NULL;
	return (argc);
}
/**
 * main - function that does it
 * Return: Always 0
 */
int main(void)
{
	char *cmd = NULL;
	size_t m = 0;
	ssize_t cmd_size;
	char *args[MAX_ARGUMENTS + 1];
	int argc;

	while (1)
	{
		print_prompt();
		cmd_size = getline(&cmd, &m, stdin);
		if (cmd_size == -1)
		{
			break;
		}
		if (cmd[cmd_size - 1] == '\n')
		{
			cmd[cmd_size - 1] = '\0';
		}
		argc = parse_command(cmd, args);
		if (argc > 0)
		{
			execute(args[0], args);
		}
	}
	free(cmd);
	return (0);
}
