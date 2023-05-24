#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

char* my_getline() {
    static char buffer[BUFFER_SIZE];
    static int buffer_index = 0;
    static int characters_read = 0;
    static int end_of_file = 0;

    if (end_of_file) {
        return NULL;
    }

    char* line = malloc(BUFFER_SIZE);
    if (line == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    int line_index = 0;
    int read_count = 0;

    while (1) {
        if (buffer_index >= characters_read) {
            buffer_index = 0;
            characters_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (characters_read == -1) {
                perror("Error reading input");
                exit(1);
            } else if (characters_read == 0) {
                end_of_file = 1;
                break;
            }
        }

        char current_char = buffer[buffer_index++];
        line[line_index++] = current_char;

        if (current_char == '\n') {
            break;
        }

        read_count++;
        if (read_count >= BUFFER_SIZE - 1) {
            break;
        }
    }

    line[line_index] = '\0';
    return line;
}

int main() {
    char* line;

    while ((line = my_getline()) != NULL) {
        printf("Input: %s", line);
        free(line);
    }

    return 0;
}
