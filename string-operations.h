#ifndef MATRIX_MULTIPLICATION_STRING_OPERATIONS_H
#define MATRIX_MULTIPLICATION_STRING_OPERATIONS_H

#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
    int length = (int) strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

char *char_to_string(char c) {
    char *str = (char *) malloc(2 * sizeof(char));
    str[0] = c;
    str[1] = '\0';
    return str;
}

char *remove_substring(char *str, const char *substr) {
    char *p = strstr(str, substr);
    if (p != NULL) {
        size_t len = strlen(substr);
        memmove(p, p + len, strlen(p + len) + 1);
    }
    return str;
}

char **split_string(char *str, const char *delim, int *num_tokens) {
    char **tokens = NULL;
    int max_tokens = 1;
    char *token = strtok(str, delim);

    tokens = malloc(max_tokens * sizeof(char *));
    if (tokens == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    while (token != NULL) {
        tokens[*num_tokens] = token;
        (*num_tokens)++;
        if (*num_tokens >= max_tokens) {
            max_tokens *= 2;
            tokens = realloc(tokens, max_tokens * sizeof(char *));
            if (tokens == NULL) {
                perror("Failed to allocate memory");
                exit(1);
            }
        }
        token = strtok(NULL, delim);
    }

    return tokens;
}

int *string_array_to_int_array(char **str_array, int num_tokens) {
    int *int_array = malloc(num_tokens * sizeof(int));
    if (int_array == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    char *ptr;
    for (int i = 0; i < num_tokens; i++) {
        int_array[i] = (int) strtol(str_array[i], &ptr, 10);
    }

    return int_array;
}

#endif
