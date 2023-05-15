#include <stdio.h>
#include <unistd.h>
/**
 * main - prints pid
 * Return: Always 0
 */
int main(void)
{
	pid_t pid;
	pid_t ppid;

	pid = getpid();
	ppid = getppid();

	printf("my pid is %u\n", pid);
	printf("ze ppid is %u\n", ppid);
	return (0);
}
