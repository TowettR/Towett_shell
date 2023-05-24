#include <stdio.h>
#include <string.h>
int main(void)
{
	char p[] = "The bright is very future";
	char d[] = " ";
	
	char *token = strtok(p, d);

	while (token != NULL)
	{
		printf("%s\n", token);
		token = strtok(NULL, d);
	}
	return (0);
}
