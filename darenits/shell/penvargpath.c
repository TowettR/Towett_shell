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

char* get_command_path(char* command) {
    char* path = getenv("PATH");

    if (path == NULL) {
        return NULL;
    }

    char* token = strtok(path, ":");

    while (token != NULL) {
        char* command_path = malloc(strlen(token) + strlen(command) + 2); // +2 for '/' and '\0'
        sprintf(command_path, "%s/%s", token, command);

        if (access(command_path, X_OK) == 0) {
            return command_path;
        }

        free(command_path);

        token = strtok(NULL, ":");
    }

    return NULL;
}

void execute_command(char* command, char* arguments[]) {
    char* command_path = get_command_path(command);

    if (command_path == NULL) {
        printf("Command not found: %s\n", command);
        return;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        execve(command_path, arguments, environ);
        perror("exec");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }

    free(command_path);
}

int parse_command(char* command, char* arguments[]) {
    int argc = 0;

    // Tokenize the command line into arguments
    char* token = strtok(command, " ");
    while (token != NULL && argc < MAX_ARGUMENTS) {
        arguments[argc++] = token;
        token = strtok(NULL, " ");
    }

    arguments[argc] = NULL;  // Set the last argument as NULL for execve

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
