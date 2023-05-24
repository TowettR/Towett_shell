#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

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

void change_directory(const char* path) {
    char current_path[PATH_MAX];

    if (strcmp(path, "-") == 0) {
        const char* previous_directory = getenv("OLDPWD");
        if (previous_directory != NULL) {
            if (chdir(previous_directory) == 0) {
                setenv("OLDPWD", getcwd(current_path, sizeof(current_path)), 1);
            } else {
                fprintf(stderr, "Failed to change directory to previous directory\n");
            }
        } else {
            fprintf(stderr, "No previous directory set\n");
        }
    } else {
        if (chdir(path) == 0) {
            setenv("OLDPWD", getcwd(current_path, sizeof(current_path)), 1);
        } else {
            fprintf(stderr, "Failed to change directory: %s\n", path);
        }
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

        if (strncmp(command, "cd", 2) == 0) {
            // Handle cd command
            char* path = strtok(command + 3, " ");
            if (path == NULL) {
                path = getenv("HOME");
            }
            change_directory(path);
        } else {
            // Execute regular command
            execute_command(command);
        }
    }

    return 0;
}
