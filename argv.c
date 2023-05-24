#include <stdio.h>
#include <stdlib.h>

int main(char *argv[]) {
  for (int i = 0; argv[i] != NULL; i++) {
    printf("%s\n", argv[i]);
  }
  return 0;
}
