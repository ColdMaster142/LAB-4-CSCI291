#include <stdio.h>  // Include the standard library for file input/output functions
#include <stdlib.h>  // Include the standard library for utility functions like strtod() and exit()
#include <ctype.h>   // Include the ctype library (although it's not used in this code)


// Function prototypes
int isValidFloat(const char *str);  // Declare a function to check if a string can be converted to a valid float
void processFile(const char *inputforfile, const char *outputforfile);  // Declare a function to read from the input file and write valid floats to the output file

int main() {
    const char *inputforfile = "data.txt";  // Set the name of the input file
    const char *outputforfile = "valid_data.txt";  // Set the name of the output file

    processFile(inputforfile, outputforfile);  // Call the function to process the input and output files

    return 0;  // Exit the main function successfully
}

// Function to determine if a string represents a valid float
int isValidFloat(const char *str) {
    char *endPtr;  // Pointer used to track the end of the parsed number

    strtod(str, &endPtr);  // Try to convert the string into a float and update the pointer to the first invalid character

    // Ensure the string is not empty and that the entire string is a valid float
    return (*str != '\0' && *endPtr == '\0');  // Return true if the string was fully parsed as a float, otherwise false
}

// Function to process the input file, writing valid floats to the output file
void processFile(const char *inputforfile, const char *outputforfile) {
    FILE *inputFile = fopen(inputforfile, "r");  // Open the input file for reading
    FILE *outputFile = fopen(outputforfile, "w");  // Open the output file for writing

    char presentstring[100];  // Buffer to hold each word read from the input file
    int nonvalidfloats = 0;  // Counter for the number of invalid floats found

    // Check if either file failed to open
    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening file");  // Display an error message if file opening fails
        exit(EXIT_FAILURE);  // Exit the program with a failure status
    }

    // Read each word from the input file until the end of file (EOF)
    while (fscanf(inputFile, "%s", presentstring) != EOF) {  // Read a string from the input file
        // Check if the current string is a valid float
        if (isValidFloat(presentstring)) {  // If the word is a valid float
            fprintf(outputFile, "%s\n", presentstring);  // Write the valid float to the output file
        } else {
            nonvalidfloats++;  // Increment the counter for invalid float entries
        }
    }

    // Check if an error occurred while reading the input file
    if (ferror(inputFile)) {
        perror("Error reading from input file");  // Display an error message if reading fails
        fclose(inputFile);  // Close both files before exiting
        fclose(outputFile);
        exit(EXIT_FAILURE);  // Exit with a failure status
    }

    // Close both files after processing
    fclose(inputFile);  // Close the input file
    fclose(outputFile);  // Close the output file

    // Print the total number of invalid float entries
    printf("Number of invalid float values: %d\n", nonvalidfloats);  // Display the count of invalid float values found
}
