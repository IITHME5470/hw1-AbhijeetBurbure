#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototype
void print_to_file(int n, double **A, int format_flag);

int main() {
    // Read n from the file "input.in"
    FILE *input_file = fopen("input.in", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Could not open input.in\n");
        return 1;
    }

    int n;
    fscanf(input_file, "%d", &n);  // Read a single integer (n)
    fclose(input_file);

    // Dynamically allocate a 2D array for n x n matrix
    double **A = (double **)malloc(n * sizeof(double *));
    int i;
    for (i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
    }

    // Fill the array A[i][j] = i + j
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }

    // Call the function to write the array to files in both formats
    print_to_file(n, A, 0);  // Write in ASCII format
    print_to_file(n, A, 1);  // Write in binary format

    // Free the dynamically allocated memory
    for (i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}

void print_to_file(int n, double **A, int format_flag) {
    // Generate the file name
    char filename[50];
    snprintf(filename, sizeof(filename), "array_%06d_%s.out", n, format_flag == 0 ? "asc" : "bin");

    FILE *output_file = fopen(filename, format_flag == 0 ? "w" : "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return;
    }

    if (format_flag == 0) {
        // Write in ASCII format with 15-decimal places precision
        int i, j;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                fprintf(output_file, "%.15f ", A[i][j]);
            }
            fprintf(output_file, "\n");
        }
    } else if (format_flag == 1) {
        // Write in binary format
        int i;
        for (i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, output_file);
        }
    }

    fclose(output_file);
}
