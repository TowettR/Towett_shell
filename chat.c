#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void split_arguments(char *input, char *argv[], int *argc) {
    // Function to split the input string into individual arguments
    // and populate the argv array with the arguments
    // argc is updated with the number of arguments
    // This function is similar to the previous implementation
    // and is responsible for tokenizing the input string.
    // It uses strtok to split the input based on space delimiter
}

int main(void) {
    const char *cmd = "$$";
    char *buf = NULL;
    size_t n = 0;
    ssize_t getl;

    while (1) {
        write(1, cmd, 2);

        getl = getline(&buf, &n, stdin);
        if (getl == -1) {
            perror("Error: getline error");
            free(buf);
            exit(1);
        }
        if (buf[getl - 1] == '\n') {
            buf[getl - 1] = '\0';
        }

        char *argv[100];
        int argc;
        split_arguments(buf, argv, &argc);

        if (strcmp(argv[0], "exit") == 0) {
            free(buf);
            exit(0);
        }

        char *path = getenv("PATH");
        char *token = strtok(path, ":");
        int command_exists = 0;
        while (token != NULL) {
            char command_path[100];
            snprintf(command_path, sizeof(command_path), "%s/%s", token, argv[0]);
            if (access(command_path, X_OK) == 0) {
                command_exists = 1;
                break;
            }
            token = strtok(NULL, ":");
        }

        if (!command_exists) {
            printf("Error: Command not found\n");
        } else {
            pid_t thepid = fork();

            if (thepid == -1) {
                perror("Error: forking");
                free(buf);
                exit(1);
            } else if (thepid == 0) {
                int m = execvp(argv[0], argv);
                if (m == -1) {
                    perror("Error: execution failed");
                    free(buf);
                    exit(1);
                }
            } else {
                int status;
                waitpid(thepid, &status, 0);
            }
        }
    }

    free(buf);
    return 0;
}
