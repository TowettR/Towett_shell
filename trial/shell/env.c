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
    if (strcmp(command, "exit") == 0) {
        exit(0);
    } else if (strcmp(command, "env") == 0) {
        char** env = environ;
        while (*env != NULL) {
            printf("%s\n", *env);
            env++;
        }
    } else {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            execvp(command, arguments);
            perror("exec");
            exit(1);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

int parse_command(char* command, char* arguments[]) {
    int argc = 0;

    // Tokenize the command line into arguments
    char* token = strtok(command, " ");
    while (token != NULL && argc < MAX_ARGUMENTS) {
        arguments[argc++] = token;
        token = strtok(NULL, " ");
    }

    arguments[argc] = NULL;  // Set the last argument as NULL for execvp

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
    }

    free(line);

    return 0;
}
