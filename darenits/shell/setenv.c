#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

extern char** environ; // External variable holding the environment

void print_prompt() {
    printf("simple_shell$ ");
}

void execute_command(char* command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        char* argv[] = {command, NULL};
        execve(command, argv, environ);
        perror("exec");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

void set_environment_variable(const char* variable, const char* value) {
    if (setenv(variable, value, 1) != 0) {
        fprintf(stderr, "Failed to set environment variable: %s\n", variable);
    }
}

void unset_environment_variable(const char* variable) {
    if (unsetenv(variable) != 0) {
        fprintf(stderr, "Failed to unset environment variable: %s\n", variable);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        print_prompt();

        if (fgets(command, sizeof(command), stdin) == NULL) {
            // End of file (Ctrl+D) or error in input
            break;
        }

        // Remove the trailing newline character
        command[strcspn(command, "\n")] = '\0';

        if (strncmp(command, "setenv", 6) == 0) {
            // Handle setenv command
            char* variable = strtok(command + 7, " ");
            char* value = strtok(NULL, " ");
            if (variable != NULL && value != NULL) {
                set_environment_variable(variable, value);
            } else {
                fprintf(stderr, "Invalid setenv command syntax\n");
            }
        } else if (strncmp(command, "unsetenv", 8) == 0) {
            // Handle unsetenv command
            char* variable = strtok(command + 9, " ");
            if (variable != NULL) {
                unset_environment_variable(variable);
            } else {
                fprintf(stderr, "Invalid unsetenv command syntax\n");
            }
        } else {
            // Execute regular command
            execute_command(command);
        }
    }

    return 0;
}
