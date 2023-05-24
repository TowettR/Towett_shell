#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(void)
{
	pid_t my_pid;
	int status;
	char *argv[] = {"/bin/ls", "-l", "/usr/", NULL};

	execve(argv[0], argv, NULL);
	if (execve(argv[0], argv, NULL) == -1)
	{
		perror("no way, error");
	}
	my_pid = fork();
	if (my_pid == 0)
	{
		printf("I am the child");
		sleep(10);
	}
	if (my_pid == -1)
		perror("you have made a mistake");
	else
	{
		wait(&status);
		printf("I have let you go, child");
	}
	return (0);
}
