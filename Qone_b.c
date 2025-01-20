#include <stdio.h>

int main() {
    // Open the input file
    FILE *inputFile = fopen("input.in", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    // Read the value of n
    int n;
    fscanf(inputFile, "%d", &n);
    fclose(inputFile);

    // Calculate memory size
    unsigned long long memorySizeBytes = (unsigned long long)n * n * sizeof(double);

    // Print the result
    printf("Array size (n x n): %d x %d\n", n, n);
    printf("Memory size in MB: %.2f\n", memorySizeBytes / (1024.0 * 1024.0));

    return 0;
}
