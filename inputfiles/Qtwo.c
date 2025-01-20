#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
void readMatrix(const char *filename, int n, double matrix[n][n]);
void readVector(const char *filename, int n, double vector[n]);
int isEigenvector(int n, double matrix[n][n], double vector[n], double *eigenvalue);
void appendEigenvalueToFile(const char *filename, double eigenvalue);

double absolute(double value) {
    return value < 0 ? -value : value;
}

int main() {
    int n;
    char matrixFilename[100];
    char vectorFilename[100];

    // Open the input file containing multiple values of n
    FILE *inputFile = fopen("input.in", "r");
    if (!inputFile) {
        printf("Error: Could not open input.in\n");
        return 1;
    }

    // Read and process each value of n from the input file
    while (fscanf(inputFile, "%d", &n) != EOF) {
        printf("\nProcessing matrix and vectors for n = %d\n", n);

        // Construct the matrix filename for the current n
        sprintf(matrixFilename, "mat_%06d.in", n);

        // Allocate memory for the matrix and read it
        double matrix[n][n];
        readMatrix(matrixFilename, n, matrix);

        // Loop through vector files
        int vecnum;
        for (vecnum = 1;; vecnum++) {
            sprintf(vectorFilename, "vec_%06d_%06d.in", n, vecnum);

            FILE *vecFile = fopen(vectorFilename, "r");
            if (!vecFile) {
                break;  // No more vector files found for this n
            }
            fclose(vecFile);

            // Allocate memory for the vector and read it
            double vector[n];
            readVector(vectorFilename, n, vector);

            // Check if the vector is an eigenvector
            double eigenvalue;
            if (isEigenvector(n, matrix, vector, &eigenvalue)) {
                printf("%s : Yes : %.6f\n", vectorFilename, eigenvalue);
                appendEigenvalueToFile(vectorFilename, eigenvalue);
            } else {
                printf("%s : Not an eigenvector\n", vectorFilename);
            }
        }
    }

    fclose(inputFile);  // Ensure to close the input file after processing all values of n
    return 0;  // Ensure main ends with a return statement
}


void readMatrix(const char *filename, int n, double matrix[n][n]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }

    // Increased buffer size to handle large rows for n = 50, 80
    char line[10000];  // Increase buffer size to 10,000 characters for large rows
    int i, j;

    for (i = 0; i < n; i++) {
        if (!fgets(line, sizeof(line), file)) {
            printf("Error: Failed to read line %d in matrix file %s\n", i, filename);
            fclose(file);
            exit(1);
        }

        // Debugging output to check if we are reading large lines correctly
        // printf("Reading row %d: %s\n", i, line);  // Uncomment for debugging

        char *token = strtok(line, ",\n");  // Tokenize by commas or newlines
        for (j = 0; j < n; j++) {
            if (token == NULL) {
                printf("Error: Not enough values in row %d of matrix file %s\n", i, filename);
                fclose(file);
                exit(1);
            }

            if (sscanf(token, "%lf", &matrix[i][j]) != 1) {
                printf("Error: Invalid format in matrix file %s at position (%d,%d)\n", filename, i, j);
                fclose(file);
                exit(1);
            }

            // Move to the next token (next number in the row)
            token = strtok(NULL, ",\n");
        }
    }

    fclose(file);
}
void readVector(const char *filename, int n, double vector[n]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }

    char line[2048];  // Increase buffer size for large lines
    if (!fgets(line, sizeof(line), file)) {
        printf("Error: Failed to read the vector from file %s\n", filename);
        fclose(file);
        exit(1);
    }

    // Tokenize by commas, spaces, or newlines
    char *token = strtok(line, ", \n");  // Tokenize by commas, spaces, or newlines
    int i = 0;
    while (token != NULL && i < n) {
        if (sscanf(token, "%lf", &vector[i]) != 1) {
            printf("Error: Invalid format in vector file %s at position %d\n", filename, i);
            fclose(file);
            exit(1);
        }
        i++;
        token = strtok(NULL, ", \n");  // Move to the next token
    }

    // If the number of elements in the vector doesn't match 'n', print an error
    if (i != n) {
        printf("Error: Incorrect number of elements in vector file %s (expected %d, got %d)\n", filename, n, i);
        fclose(file);
        exit(1);
    }

    fclose(file);
}




int isEigenvector(int n, double matrix[n][n], double vector[n], double *eigenvalue) {
    double result[n];
    double firstComponent = 0.0;

    // Compute matrix-vector product
    int i, j;
    for (i = 0; i < n; i++) {
        result[i] = 0.0;
        for (j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    // Check for proportionality and determine eigenvalue
    for (i = 0; i < n; i++) {
        if (absolute(vector[i]) > 0.000001) {
            double ratio = result[i] / vector[i];
            if (absolute(firstComponent) < 0.000001) {
                firstComponent = ratio;
            } else if (absolute(ratio - firstComponent) > 0.000001) {
                return 0; // Not an eigenvector
            }
        } else if (absolute(result[i]) > 0.000001) {
            return 0; // Not an eigenvector
        }
    }

    *eigenvalue = firstComponent;
    return 1; // Is an eigenvector
}

void appendEigenvalueToFile(const char *filename, double eigenvalue) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Error: Could not open %s for appending\n", filename);
        exit(1);
    }

    fprintf(file, "\n%.6f", eigenvalue);
    fclose(file);
}
