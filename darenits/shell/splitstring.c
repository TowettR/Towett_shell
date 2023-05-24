#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 100

int split_string(char* string, char** tokens) {
    int num_tokens = 0;
    int string_length = strlen(string);
    int token_start = -1;
    int token_length = 0;

    for (int i = 0; i <= string_length; i++) {
        if (isspace(string[i]) || string[i] == '\0') {
            // Found a whitespace or end of string
            if (token_start != -1) {
                tokens[num_tokens] = malloc(token_length + 1);
                if (tokens[num_tokens] == NULL) {
                    perror("Memory allocation error");
                    exit(1);
                }

                strncpy(tokens[num_tokens], string + token_start, token_length);
                tokens[num_tokens][token_length] = '\0';

                num_tokens++;
                token_start = -1;
                token_length = 0;
            }
        } else {
            // Found a non-whitespace character
            if (token_start == -1) {
                token_start = i;
            }
            token_length++;
        }
    }

    return num_tokens;
}

int main() {
    char* input = NULL;
    size_t input_size = 0;
    char* tokens[MAX_TOKENS];

    printf("Enter a string: ");
    ssize_t num_chars = getline(&input, &input_size, stdin);
    if (num_chars == -1) {
        perror("Error reading input");
        exit(1);
    }

    int num_tokens = split_string(input, tokens);

    for (int i = 0; i < num_tokens; i++) {
        printf("Token %d: %s\n", i, tokens[i]);
        free(tokens[i]);
    }

    free(input);

    return 0;
}
