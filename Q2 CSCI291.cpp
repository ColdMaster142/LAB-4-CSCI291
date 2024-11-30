#include <stdio.h>    // Library for input and output operations
#include <stdlib.h>   // Library for memory allocation and utility functions

#define WIDTH 512    // Set the image width to 512 pixels
#define HEIGHT 512   // Set the image height to 512 pixels

// Function to load pixel data from a PGM text format file
int loadPGMText(const char *filePath, unsigned char *imgData, int imgWidth, int imgHeight) {
    FILE *file = fopen(filePath, "r"); // Open the file in read mode
    if (!file) { // If the file cannot be opened
        printf("Error: Unable to open file %s\n", filePath); // Display error message
        return -1; // Return error code
    }

    char format[3]; // Array to store the PGM format identifier
    fscanf(file, "%s\n", format); // Read the format (e.g., P2)
    if (format[0] != 'P' || format[1] != '2') { // Ensure the file is in P2 format
        printf("Error: Incorrect PGM format\n"); // Print error for invalid format
        fclose(file); // Close the file
        return -2; // Return error code
    }

    // Skip over comments in the file
    char character = getc(file); // Read a character
    while (character == '#') { // While the character is a comment
        while (getc(file) != '\n'); // Skip characters until the end of the line
        character = getc(file); // Read the next character
    }
    ungetc(character, file); // Put the character back for future use

    // Read image dimensions and the maximum pixel value
    int readWidth, readHeight, maxValue; // Variables for width, height, and max pixel value
    fscanf(file, "%d %d\n%d\n", &readWidth, &readHeight, &maxValue); // Parse the dimensions and max value
    if (readWidth != imgWidth || readHeight != imgHeight) { // Check if the dimensions match
        printf("Error: Dimension mismatch\n"); // Display error message
        fclose(file); // Close the file
        return -3; // Return error code
    }

    // Load the pixel values into the imgData array
    for (int idx = 0; idx < imgWidth * imgHeight; idx++) { // Loop through each pixel
        int pixelValue; // Temporary variable to hold pixel data
        fscanf(file, "%d", &pixelValue); // Read the pixel value
        *(imgData + idx) = (unsigned char)pixelValue; // Store the value in the image array
    }

    fclose(file); // Close the file after reading
    return 0; // Return success code
}

// Function to save pixel data into a PGM text format file
int savePGMText(const char *filePath, unsigned char *imgData, int imgWidth, int imgHeight) {
    FILE *file = fopen(filePath, "w"); // Open the file in write mode
    if (!file) { // If the file cannot be created
        printf("Error: Unable to create file %s\n", filePath); // Display error message
        return -1; // Return error code
    }

    // Write the header details
    fprintf(file, "P2\n"); // Write the PGM format identifier
    fprintf(file, "# Created using LSB Steganography\n"); // Add a comment for the source
    fprintf(file, "%d %d\n", imgWidth, imgHeight); // Write the image dimensions
    fprintf(file, "255\n"); // Write the maximum pixel intensity

    // Write the pixel data
    for (int idx = 0; idx < imgWidth * imgHeight; idx++) { // Traverse each pixel
        fprintf(file, "%d", imgData[idx]); // Write the pixel value
        if ((idx + 1) % imgWidth == 0) // Check if it’s the end of a row
            fprintf(file, "\n"); // Insert a new line after a row
        else
            fprintf(file, " "); // Otherwise, separate values with spaces
    }

    fclose(file); // Close the file after writing
    return 0; // Return success code
}

// Function to save pixel data into a binary PGM file
int savePGMBinary(const char *filePath, unsigned char *imgData, int imgWidth, int imgHeight) {
    FILE *file = fopen(filePath, "wb"); // Open the file in binary write mode
    if (!file) { // If the file cannot be created
        printf("Error: Unable to create file %s\n", filePath); // Display error message
        return -1; // Return error code
    }

    // Write the header details
    fprintf(file, "P5\n"); // Write the binary PGM format identifier
    fprintf(file, "# Created using LSB Steganography\n"); // Add a comment for the source
    fprintf(file, "%d %d\n", imgWidth, imgHeight); // Write the image dimensions
    fprintf(file, "255\n"); // Write the maximum pixel intensity

    // Write the pixel data in binary form
    fwrite(imgData, sizeof(unsigned char), imgWidth * imgHeight, file); // Write the pixel data in one go

    fclose(file); // Close the file after writing
    return 0; // Return success code
}

// Function to hide the secret image within the cover image using the LSB method
void embedSecret(unsigned char *cvrData, unsigned char *scrtData, int imgWidth, int imgHeight) {
    for (int idx = 0; idx < imgWidth * imgHeight; idx++) { // Process each pixel
        cvrData[idx] &= 0xF0; // Mask the lower 4 bits of the cover image pixel

        unsigned char scrtHighBits = (scrtData[idx] & 0xF0) >> 4; // Extract the top 4 bits of the secret image pixel

        cvrData[idx] |= scrtHighBits; // Place the secret image bits into the cover image
    }
}

// Function to retrieve the secret image from the stego image
void extractSecret(unsigned char *stegoData, unsigned char *extractedData, int imgWidth, int imgHeight) {
    for (int idx = 0; idx < imgWidth * imgHeight; idx++) { // Iterate through each pixel
        unsigned char lsb = stegoData[idx] & 0x0F; // Extract the least significant bits from the stego image

        extractedData[idx] = lsb << 4; // Shift the LSBs to the most significant bits for the secret image
    }
}

// Main function coordinating the whole process
int main() {
    const char *cvrFile = "baboon.pgm"; // Path to the cover image file
    const char *scrtFile = "farm.pgm"; // Path to the secret image file
    const char *stegoFile = "stego_image_bin.pgm"; // Path for saving the stego image
    const char *outputSecretFile = "extracted_secret.pgm"; // Path for saving the extracted secret image

    unsigned char *cvrImage = NULL, *scrtImage = NULL, *extractedImage = NULL; // Pointers for the image data
    int cvrWidth = WIDTH, cvrHeight = HEIGHT; // Dimensions for the cover image
    int scrtWidth = WIDTH, scrtHeight = HEIGHT; // Dimensions for the secret image

    // Allocate memory for the cover image data
    cvrImage = (unsigned char *)malloc(cvrWidth * cvrHeight * sizeof(unsigned char));
    if (cvrImage == NULL) { // If memory allocation fails
        printf("Error: Memory allocation failed for cover image\n");
        return -1; // Return error code
    }

    // Load the cover image pixel data
    if (loadPGMText(cvrFile, cvrImage, cvrWidth, cvrHeight) != 0) {
        free(cvrImage); // Release memory on error
        return -2; // Return error code
    }

    // Allocate memory for the secret image data
    scrtImage = (unsigned char *)malloc(scrtWidth * scrtHeight * sizeof(unsigned char));
    if (scrtImage == NULL) { // If memory allocation fails
        printf("Error: Memory allocation failed for secret image\n");
        free(cvrImage); // Free allocated memory
        return -3; // Return error code
    }

    // Load the secret image pixel data
    if (loadPGMText(scrtFile, scrtImage, scrtWidth, scrtHeight) != 0) {
        free(cvrImage); // Release memory
        free(scrtImage); // Release memory
        return -4; // Return error code
    }

    // Ensure the cover and secret images have identical dimensions
    if (cvrWidth != scrtWidth || cvrHeight != scrtHeight) {
        printf("Error: Image dimensions do not match\n");
        free(cvrImage); // Release memory
        free(scrtImage); // Release memory
        return -5; // Return error code
    }

    // Embed the secret image data into the cover image using the LSB method
    embedSecret(cvrImage, scrtImage, cvrWidth, cvrHeight);

    // Save the resulting stego image in binary format
    if (savePGMBinary(stegoFile, cvrImage, cvrWidth, cvrHeight) != 0) {
        free(cvrImage); // Release memory
        free(scrtImage); // Release memory
        return -6; // Return error code
    }

    // Allocate memory for the extracted secret image data
    extractedImage = (unsigned char *)malloc(cvrWidth * cvrHeight * sizeof(unsigned char));
    if (extractedImage == NULL) { // If memory allocation fails
        printf("Error: Memory allocation failed for extracted image\n");
        free(cvrImage); // Release memory
        free(scrtImage); // Release memory
        return -7; // Return error code
    }

    // Extract the secret image from the stego image
    extractSecret(cvrImage, extractedImage, cvrWidth, cvrHeight);

    // Save the extracted secret image to a PGM text file
    if (savePGMText(outputSecretFile, extractedImage, cvrWidth, cvrHeight) != 0) {
        free(cvrImage); // Release memory
        free(scrtImage); // Release memory
        free(extractedImage); // Release memory
        return -8; // Return error code
    }

    // Free allocated memory before terminating the program
    free(cvrImage);
    free(scrtImage);
    free(extractedImage);

    printf("Steganography completed successfully!\n"); // Print success message
    return 0; // Return success code
}
