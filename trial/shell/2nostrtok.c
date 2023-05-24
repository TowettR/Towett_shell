#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGUMENTS 10

extern char** environ; // External variable holding the environment

void print_prompt() {
    printf("simple_shell$ ");
}

void execute_command(char* command, char* arguments[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        execve(command, arguments, environ);
        perror("exec");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

int parse_command(char* command, char* arguments[]) {
    int argc = 0;
    int command_length = strlen(command);
    int arg_start = -1;
    int arg_length = 0;

    for (int i = 0; i <= command_length; i++) {
        if (command[i] == ' ' || command[i] == '\0') {
            // Found a whitespace or end of string
            if (arg_length > 0) {
                arguments[argc] = malloc(arg_length + 1);
                if (arguments[argc] == NULL) {
                    perror("Memory allocation error");
                    exit(1);
                }

                strncpy(arguments[argc], command + arg_start, arg_length);
                arguments[argc][arg_length] = '\0';

                argc++;
                arg_length = 0;
            }
            arg_start = -1;
        } else {
            // Found a non-whitespace character
            if (arg_start == -1) {
                arg_start = i;
            }
            arg_length++;
        }
    }

    arguments[argc] = NULL; // Terminate arguments with NULL pointer

    return argc;
}

int main() {
    char* line = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;
    char* arguments[MAX_ARGUMENTS + 1];

    while (1) {
        print_prompt();

        line_size = getline(&line, &line_buf_size, stdin);
        if (line_size == -1) {
            // End of file (Ctrl+D) or error in input
            break;
        }

        // Remove the trailing newline character, if present
        if (line[line_size - 1] == '\n') {
            line[line_size - 1] = '\0';
        }

        int argc = parse_command(line, arguments);

        if (argc > 0) {
            execute_command(arguments[0], arguments);
        }

        // Free the allocated argument strings
        for (int i = 0; i < argc; i++) {
            free(arguments[i]);
        }
    }

    free(line);

    return 0;
}
