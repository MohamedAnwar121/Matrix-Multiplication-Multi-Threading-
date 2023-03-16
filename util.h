#ifndef MATRIX_MULTIPLICATION_UTIL_H
#define MATRIX_MULTIPLICATION_UTIL_H


#include <stdlib.h>
#include <string.h>
#include "string-operations.h"


typedef enum {
    true = 1,
    false = 0
} boolean;

int getRowSize(char *file_first_line);

int getColSize(char *file_first_line);


void compute_time(void (*f)(), char *operation) {
    struct timeval stop, start;

    gettimeofday(&start, NULL);
    f();
    gettimeofday(&stop, NULL);

    printf("%s : Seconds taken (%lu)\n", operation, stop.tv_sec - start.tv_sec);
    printf("%s : Microseconds taken: (%lu)\n\n", operation, stop.tv_usec - start.tv_usec);
}

char *default_input_1 = "a.txt";
char *default_input_2 = "b.txt";
char *default_output_by_matrix = "c_per_matrix.txt";
char *default_output_by_row = "c_per_row.txt";
char *default_output_by_element = "c_per_element.txt";

char *input_file_1_name, *input_file_2_name;
char *output_per_matrix_filename, *output_per_row_filename, *output_per_element_filename;

void handle_file_creation(int argc, char *argv[]) {

    input_file_1_name = default_input_1;
    input_file_2_name = default_input_2;
    output_per_matrix_filename = default_output_by_matrix;
    output_per_row_filename = default_output_by_row;
    output_per_element_filename = default_output_by_element;

    if (argc == 4) {
        input_file_1_name = (char *) malloc((strlen(argv[1]) + 4) * sizeof(char));
        input_file_1_name = strdup(argv[1]);
        strcat(input_file_1_name, ".txt");

        input_file_2_name = (char *) malloc((strlen(argv[2]) + 4) * sizeof(char));
        input_file_2_name = strdup(argv[2]);
        strcat(input_file_2_name, ".txt");

        output_per_matrix_filename = (char *) malloc(
                (strlen(argv[3]) + strlen("_per_matrix.txt") * sizeof(char)));
        output_per_matrix_filename = strdup(argv[3]);
        strcat(output_per_matrix_filename, "_per_matrix.txt");

        output_per_row_filename = (char *) malloc(
                (strlen(argv[3]) + strlen("_per_row.txt") * sizeof(char)));
        output_per_row_filename = strdup(argv[3]);
        strcat(output_per_row_filename, "_per_row.txt");

        output_per_element_filename = (char *) malloc(
                (strlen(argv[3]) + strlen("_per_element.txt") * sizeof(char)));
        output_per_element_filename = strdup(argv[3]);
        strcat(output_per_element_filename, "_per_element.txt");

    }
}

int **read_array_from_file(char *file_name, int *row, int *col) {

    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    getline(&line, &len, fp);
    line[strlen(line) - 1] = '\0';

    *row = getRowSize(line);
    *col = getColSize(line);

    int **array = (int **) malloc(*row * sizeof(int *));
    int size = 0;


    for (int i = 0; i < *row; ++i) {
        array[i] = (int *) malloc(*col * sizeof(int));
        for (int j = 0; j < *col; ++j) {
            fscanf(fp, "%d", &array[i][j]);
        }
    }

    fclose(fp);

    return array;
}

int getRowSize(char *file_first_line) {
    char *temp = (char *) malloc(strlen(file_first_line) * sizeof(char));
    temp = strdup(file_first_line);

    temp = remove_substring(temp, "row=");

    char *result_string = (char *) malloc(strlen(file_first_line) * sizeof(char));
    memset(result_string, 0, strlen(file_first_line));

    int size = 0;
    while (temp[size] != ' ') {
        strcat(result_string, char_to_string(temp[size++]));
    }

    char *ptr;
    int result = (int) strtol(result_string, &ptr, 10);

    free(temp);
    free(result_string);

    return result;
}

int getColSize(char *file_first_line) {
    char *temp = (char *) malloc(strlen(file_first_line) * sizeof(char));
    temp = strdup(file_first_line);
    temp = remove_substring(temp, "col=");

    char *result_string = (char *) malloc(strlen(file_first_line) * sizeof(char));
    memset(result_string, 0, strlen(file_first_line));

    for (int i = (int) strlen(temp) - 1; temp[i] != ' '; --i)
        strcat(result_string, char_to_string(temp[i]));

    reverse_string(result_string);

    char *ptr;
    int result = (int) strtol(result_string, &ptr, 10);

    free(temp);
    free(result_string);

    return result;
}

void print2D_Array(int **array, int row, int col) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

void write_to_output_file(char *output_file_name, char *operation, int **array, int row, int col) {
    FILE *fp = fopen(output_file_name, "w");
    if (fp == NULL) {
        printf("Error creating file!\n");
        return;
    }

    fprintf(fp, "%s\n", operation);
    fprintf(fp, "row=%d col=%d\n", row, col);

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            fprintf(fp, "%d ", array[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

#endif