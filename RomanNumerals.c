#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
void readFile(const char *filename, char *data);
void writeFile(const char *filename, const char *result);
int validateRomanNumeral(const char *roman);
int romanToDecimal(const char *roman);
int performOperation(int num1, int num2, char operation);
void decimalToWords(int num, char *words);
void processInput(const char *input, char *output);

// Main function
int main() {
    char input[1000] = "";
    char output[5000] = ""; // Increased size to handle large output

    // Step 1: Read input from file
    readFile("input.txt", input);

    // Step 2: Process input and perform operations
    processInput(input, output);

    // Step 3: Write results to output file
    writeFile("Output.txt", output);

    printf("Processing complete. Check 'Output.txt' for results.\n");
    return 0;
}

// Function to read file content
void readFile(const char *filename, char *data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s.\n", filename);
        data[0] = '\0'; // Ensure `data` is empty if the file cannot be opened
        return;
    }

    data[0] = '\0'; // Initialize `data` as an empty string

    // Read each line and append it to `data` buffer
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strlen(data) + strlen(buffer) >= 1000 - 1) {
            printf("Error: Buffer overflow. File too large for data buffer.\n");
            fclose(file);
            return;
        }
        strcat(data, buffer); // Safely append the current line to `data`
    }
    fclose(file);
}

// Function to write result to file
void writeFile(const char *filename, const char *result) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to write to file %s.\n", filename);
        return;
    }
    fprintf(file, "%s", result);
    fclose(file);
}

// Validate if a string is a valid Roman numeral
int validateRomanNumeral(const char *roman) {
    int i;
    for (i = 0; roman[i] != '\0'; i++) {
        if (strchr("IVXLCDM", toupper(roman[i])) == NULL) {
            return 0; // Invalid character found
        }
    }
    return 1;
}

// Convert Roman numeral to decimal
int romanToDecimal(const char *roman) {
    int values[256] = {0};
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;
    int i;
    for (i = 0; roman[i] != '\0'; i++) {
        if (values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]];
        } else {
            total += values[roman[i]];
        }
    }
    return total;
}

// Perform arithmetic operation
int performOperation(int num1, int num2, char operation) {
    switch (operation) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : 0; // Handle division by zero
        default: return 0; // Unsupported operation
    }
}

void decimalToWords(int num, char *words) {
    // Arrays for word conversion
    char *belowTwenty[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
                           "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen",
                           "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    // Handle the special case for zero
    if (num == 0) {
        strcpy(words, "Zero");
        return;
    }

    // Initialize variables
    char temp[100] = "";  // Temporary buffer for chunks of numbers
    char result[500] = ""; // Final result buffer
    int isNegative = (num < 0); // Check if the number is negative

    if (isNegative) {
        num = -num; // Convert to positive for processing
    }

    int part, index = 0;

    // Convert the number to words in chunks of 1000
    while (num > 0) {
        part = num % 1000;
        if (part > 0) {
            char section[100] = ""; // Temporary buffer for the current chunk

            // Process hundreds place
            if (part >= 100) {
                sprintf(section, "%s Hundred ", belowTwenty[part / 100]);
                part %= 100;
            }

            // Process tens and ones places
            if (part >= 20) {
                strcat(section, tens[part / 10]);
                part %= 10;
                if (part > 0) strcat(section, " ");
            }
            if (part > 0) {
                strcat(section, belowTwenty[part]);
            }

            // Add thousands place if needed
            sprintf(temp, "%s %s %s", section, thousands[index], result);
            strcpy(result, temp);
        }

        num /= 1000; // Move to the next chunk of 1000
        index++;
    }

    // If the number was negative, prepend "Negative"
    if (isNegative) {
        sprintf(words, "Negative %s", result);
    } else {
        strcpy(words, result);
    }

    // Trim any trailing or extra spaces
    size_t len = strlen(words);
    if (len > 0 && words[len - 1] == ' ') {
        words[len - 1] = '\0'; // Remove trailing space
    }
}



// Process input and generate output
void processInput(const char *input, char *output) {
    char temp[500];
    char roman1[50], roman2[50], operation;
    int num1, num2, result;

    const char *line = input; // Pointer to current line
    while (line && *line) {
        // If sscanf returns 3 values, process the line
        if (sscanf(line, "%s %c %s", roman1, &operation, roman2) == 3) {
            if (!validateRomanNumeral(roman1) || !validateRomanNumeral(roman2)) {
                strcat(output, "Invalid Roman numeral detected.\n");
            } else {
                num1 = romanToDecimal(roman1);
                num2 = romanToDecimal(roman2);
                result = performOperation(num1, num2, operation);

                char resultWords[500];
                decimalToWords(result, resultWords);

                sprintf(temp, "%s\n", resultWords);
                strcat(output, temp);
            }
        } else {
            // Handle any invalid input line format (non-matching pattern)
            strcat(output, "Invalid input format detected.\n");
        }

        // Move to the next line by searching for a newline character
        line = strchr(line, '\n');
        if (line) {
            line++; // Skip the newline character
        }
    }
}