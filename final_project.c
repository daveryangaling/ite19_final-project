#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(char *roman); // converts Roman numeral to decimal
int isValidRoman(char *roman); // checks if Roman numeral valid
int performArithmetic(int num1, int num2, char op); // does arithmetic on two numbers
void processLine(char *line, FILE *outputFile); // process one line from file
void processFile(const char *inputFileName, const char *outputFileName); // process whole file
void numberToWords(int num, char *result); // convert number to words
void formatWords(char *str); // removes extra spaces from string

// Main function
int main()
{
    const char *inputFile = "Input.txt"; // input file name
    const char *outputFile = "Output.txt"; // output file name

    processFile(inputFile, outputFile); // process input file and write to output

    printf("Processing complete. Check %s for results.\n", outputFile); // notify user
    return 0;
}

// Convert Roman numeral string to decimal
int romanToDecimal(char *roman)
{
    // array for Roman numeral values
    int values[256] = {0};
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0, prevValue = 0;

    // loop through Roman numeral string backwards
    for (int i = strlen(roman) - 1; i >= 0; i--)
    {
        int currentValue = values[roman[i]];
        // if current value less than previous, subtract it
        if (currentValue < prevValue)
        {
            total -= currentValue;
        }
        else
        {
            total += currentValue; // else add it
        }
        prevValue = currentValue; // update previous value
    }
    return total; // return the total value
}

// Check if a string is a valid Roman numeral
int isValidRoman(char *roman)
{
    // array of valid patterns
    const char *validPatterns[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const char *ptr = roman; // pointer to go through string

    while (*ptr)
    {
        int matchFound = 0;
        // check each pattern
        for (int j = 0; j < 13; j++)
        {
            // if pattern matches, move pointer forward
            if (strncmp(ptr, validPatterns[j], strlen(validPatterns[j])) == 0)
            {
                ptr += strlen(validPatterns[j]);
                matchFound = 1;
                break;
            }
        }
        // if no match found, not valid
        if (!matchFound)
            return 0;
    }
    return 1; // valid if loop completes
}
// Perform arithmetic operation
int performArithmetic(int num1, int num2, char op)
{
    switch (op)
    {
    case '+':
        return num1 + num2; // add
    case '-':
        return (num1 < num2) ? num1 + num2 : num1 - num2; // subtract, swap if needed
    case '*':
        return (num1 < num2) ? num2 + num1 : num1 * num2; // custom multiply, swap if needed
    case '/':
        return (num2 != 0) ? num1 / num2 : -1; // divide, handle zero case
    default:
        return 0; // invalid operation
    }
}

// Convert number to words
void numberToWords(int num, char *result)
{
    // arrays for number words
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char buffer[1000] = ""; // buffer for result
    int place = 0;

    if (num == 0)
    {
        strcpy(result, "Zero"); // zero case
        return;
    }

    // process each 1000s chunk
    while (num > 0)
    {
        int chunk = num % 1000;
        num /= 1000;

        if (chunk > 0)
        {
            char chunkStr[100] = "";

            if (chunk / 100 > 0)
            {
                sprintf(chunkStr + strlen(chunkStr), "%s Hundred ", ones[chunk / 100]); // hundreds place
            }
            if (chunk % 100 >= 10 && chunk % 100 < 20)
            {
                sprintf(chunkStr + strlen(chunkStr), "%s ", teens[chunk % 100 - 10]); // teens
            }
            else
            {
                if (chunk % 100 / 10 > 0)
                {
                    sprintf(chunkStr + strlen(chunkStr), "%s ", tens[chunk % 100 / 10]); // tens place
                }
                if (chunk % 10 > 0)
                {
                    sprintf(chunkStr + strlen(chunkStr), "%s ", ones[chunk % 10]); // ones place
                }
            }

            if (place > 0)
            {
                sprintf(chunkStr + strlen(chunkStr), "%s ", thousands[place]); // thousands place
            }

            char temp[1000];
            sprintf(temp, "%s%s", chunkStr, buffer);
            strcpy(buffer, temp); // combine with buffer
        }
        place++;
    }
    formatWords(buffer); // remove extra spaces
    strcpy(result, buffer); // copy to result
}

// Remove extra spaces
void formatWords(char *str)
{
    char *src = str, *dst = str;
    while (*src)
    {
        // skip extra spaces
        while (*src == ' ' && (dst == str || *(dst - 1) == ' '))
            src++;
        *dst++ = *src++;
    }
    if (dst > str && *(dst - 1) == ' ')
        dst--;
    *dst = '\0'; // null-terminate
}

// Process a single line from input
void processLine(char *line, FILE *outputFile)
{
    char roman1[100], roman2[100], op;
    int num1, num2, result;
    char resultInWords[1000];

    if (sscanf(line, "%s %c %s", roman1, &op, roman2) == 3) // read line
    {
        if (isValidRoman(roman1) && isValidRoman(roman2)) // check valid
        {
            num1 = romanToDecimal(roman1);
            num2 = romanToDecimal(roman2);
            result = performArithmetic(num1, num2, op);
            numberToWords(result, resultInWords); // convert to words
            fprintf(outputFile, "%s\n", resultInWords); // write result
        }
        else
        {
            fprintf(outputFile, "Invalid Roman numeral\n");
        }
    }
    else
    {
        fprintf(outputFile, "Invalid input format\n");
    }
}

// Process input file
void processFile(const char *inputFileName, const char *outputFileName)
{
    FILE *inputFile = fopen(inputFileName, "r"); // open input file
    FILE *outputFile = fopen(outputFileName, "w"); // open output file

    if (inputFile == NULL || outputFile == NULL)
    {
        printf("Error opening file.\n"); // error opening file
        return;
    }

    char line[256];
    // process each line in input file
    while (fgets(line, sizeof(line), inputFile))
    {
        processLine(line, outputFile);
    }

    fclose(inputFile); // close input file
    fclose(outputFile); // close output file
}
