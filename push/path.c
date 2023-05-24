#include "shell.h"
char * const *environ;
/**
 * print_prompt - function that displays prompt to user
 * Return: $$
 */

void pathprompt(void)
{
	char *prompt = "$$";

	write(1, prompt, 2);
}
/**
 * get_path - function that gets the path from environment
 * @buf: pointer to the command storage
 * Return: 0
 */
char *get_path(char *buf)
{
	char *path = getenv("PATH");
	char *token;

	if (path == NULL)
	{
		return (NULL);
	}
	token = strtok(path, ":");
	while (token != NULL)
	{
		size_t length = strlen(token);
		size_t com_length = strlen(buf);
		char *command_path = malloc(length + 1 + com_length + 1);

		if (command_path == NULL)
		{
			perror("Error:malloc issue");
			exit(1)
		}
		memcpy(command_path, token, length);
		command_path[length] = '/';
		memcpy(command_path + length + 1, buf, com_length);
		command_path[length + 1 + com_length] = '\0';
		if (access(command_path, X_OK) == 0)
		{
			return (command_path);
		}
		free(command_path);
		token = strtok(NULL, ":");
	}
	return (NULL);
	}
/**
 * pathexecute - function that executes the command
 * @buf: pointer to command storage
 * @argv: command input by user
 * Return: output
 */
void pathexecute(char *buf, char *argv[])
{
	char *path = get_path(buf);
	pid_t thepid;

	if (path == NULL)
	{
		perror("Error:no command");
		exit(1);
	}
	thepid = fork();
	if (thepid == -1)
	{
		perror("Error:forking");
		exit(1);
	}
	else if (thepid == 0)
	{
		execve(path, argv, environ);
		perror("Error:execution");
		exit(1);
	}
	else
	{
		int status;

		waitpid(thepid, &status, 0);
	}
	free(path);
}
/**
 * pathparse - function that parses commands
 * @buf: pointer to command storage
 * @argv: command input by user
 * Return: argc
 */
int pathparse(char *buf, char *argv[])
{
	int argc = 0;
	char *token = strtok(buf, " ");

	while (token != NULL && argc < MAX_ARGUMENTS)
	{
		argv[argc++] = token;

		token = strtok(NULL, " ");
	}
	argv[argc] = NULL;
	return (argc);
}
/**
 * pathmain - function that does it all
 * Return: Always 0
 */
int pathmain(void)
{
	char *buf = NULL;
	size_t n = 0;
	ssize_t getl;
	char *argv[MAX_ARGUMENTS + 1];
	int argc;

	while (1)
	{
		char *prompt = "$$";

		write(1, prompt, 2);
		getl = getline(&buf, &n, stdin);
		if (getl == -1)
		{
			break;
		}
		if (buf[getl - 1] == '\n')
		{
			buf[getl - 1] = '\0';
		}
		argc = pathparse(buf, argv);

		if (argc > 0)
		{
			pathexecute(argv[0], argv);
		}
	}
	free(buf);
	return (0);
}
