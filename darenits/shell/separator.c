#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_COMMANDS 10

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
        execvp(command, argv);
        perror("exec");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char* commands[MAX_COMMANDS];

    while (1) {
        print_prompt();

        if (fgets(command, sizeof(command), stdin) == NULL) {
            // End of file (Ctrl+D) or error in input
            break;
        }

        // Remove the trailing newline character
        command[strcspn(command, "\n")] = '\0';

        // Tokenize the command line by semicolon (;)
        char* token = strtok(command, ";");
        int num_commands = 0;

        // Store each command in an array
        while (token != NULL && num_commands < MAX_COMMANDS) {
            commands[num_commands++] = token;
            token = strtok(NULL, ";");
        }

        // Execute each command
        for (int i = 0; i < num_commands; i++) {
            execute_command(commands[i]);
        }
    }

    return 0;
}
