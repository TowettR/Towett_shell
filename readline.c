#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	size_t n = 34;
	char *ptr = NULL;

	printf("$");
	getline(&ptr, &n, stdin);

	printf("%s\n", ptr);

	free(ptr);

	return (0);
}
