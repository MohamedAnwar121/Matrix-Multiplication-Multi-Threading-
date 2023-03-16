#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "util.h"

typedef struct {
    int row;
    int col;
} position;

int input1_row, input1_col;
int input2_row, input2_col;

int **mat_1, **mat_2, **output;

void read_arrays() {

    mat_1 = read_array_from_file(input_file_1_name, &input1_row, &input1_col);
    mat_2 = read_array_from_file(input_file_2_name, &input2_row, &input2_col);

    if (input1_col != input2_row) {
        perror("Incompatible array sizes");
        exit(0);
    }

}

void clear_arrays() {
    for (int i = 0; i < input1_row; ++i) free(mat_1[i]);
    free(mat_1);

    for (int i = 0; i < input2_row; ++i) free(mat_2[i]);
    free(mat_2);
}

void clear_file_names(int argc) {
    if (argc == 4){
        free(input_file_1_name);
        free(input_file_2_name);
        free(output_per_element_filename);
        free(output_per_matrix_filename);
        free(output_per_row_filename);
    }
}


void initialize_output() {
    output = (int **) malloc(input1_row * sizeof(int *));
    for (int i = 0; i < input1_row; ++i) {
        output[i] = (int *) malloc(input2_col * sizeof(int));
        for (int j = 0; j < input2_col; ++j)
            output[i][j] = 0;
    }
}

void clear_output(){
    for (int i = 0; i < input1_row; ++i) free(output[i]);
    free(output);
}

void *thread_by_matrix() {
    for (int i = 0; i < input1_row; i++) {
        for (int j = 0; j < input2_col; j++) {
            for (int k = 0; k < input1_col; k++) {
                output[i][j] += mat_1[i][k] * mat_2[k][j];
            }
        }
    }
    return NULL;
}

void ByMatrix() {
    initialize_output();

    pthread_t thread;
    pthread_create(&thread, NULL, &thread_by_matrix, NULL);
    pthread_join(thread, NULL);

    write_to_output_file(output_per_matrix_filename,
                         "Method: A thread per matrix", output, input1_row, input2_col);

    printf("Number of threads = %d\n",1);

    clear_output();
}

void *thread_by_row(void *arg) {

    int i = *(int *) arg;
    for (int j = 0; j < input2_col; j++) {
        for (int k = 0; k < input1_col; k++) {
            output[i][j] += mat_1[i][k] * mat_2[k][j];
        }
    }
    free(arg);
    return NULL;
}


void ByRow() {
    initialize_output();

    pthread_t thread[input1_row];
    for (int i = 0; i < input1_row; ++i) {
        int *temp = (int *) malloc(sizeof(int));
        *temp = i;
        if (pthread_create(&thread[i], NULL, thread_by_row, temp) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < input1_row; ++i) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("Failed to stop thread");
        }
    }

    write_to_output_file(output_per_row_filename,
                         "Method: A thread per row", output, input1_row, input2_col);

    printf("Number of threads = %d\n",input1_row);

    clear_output();
}

void *thread_by_element(void *arg) {
    position p = *(position *) arg;
    int i = p.row, j = p.col;

    for (int k = 0; k < input1_col; k++) {
        output[i][j] += mat_1[i][k] * mat_2[k][j];
    }

    free(arg);
    return NULL;
}

void ByElement() {
    initialize_output();

    pthread_t thread[input1_row][input2_col];

    for (int i = 0; i < input1_row; ++i) {
        for (int j = 0; j < input2_col; ++j) {
            position *p = (position *) malloc(sizeof(position));
            p->row = i , p->col = j;
            if (pthread_create(&thread[i][j], NULL, thread_by_element, p) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (int i = 0; i < input1_row; ++i) {
        for (int j = 0; j < input2_col; ++j) {
            if (pthread_join(thread[i][j], NULL) != 0) {
                perror("Failed to stop thread");
            }
        }
    }

    write_to_output_file(output_per_element_filename,
                         "Method: A thread per element", output, input1_row, input2_col);

    printf("Number of threads = %d\n",input1_row * input2_col);

    clear_output();
}


int main(int argc, char *argv[]) {

    handle_file_creation(argc, argv);

    read_arrays();

    compute_time(ByMatrix, "A thread per matrix");
    compute_time(ByRow, "A thread per row");
    compute_time(ByElement, "A thread per element");

    clear_arrays();
    clear_file_names(argc);
    return 0;
}

