#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_SUCCESS                   0
#define ERROR_INVALID_ARGUMENT_ENTERED  1
#define ERROR_BAD_INPUT_SIZE            2
#define ERROR_MALLOC_FAILED             3

#define CHECK_MINIMUM_INPUT_SIZE        1
#define CHECK_INPUT_NUMBERS             1
#define CHECK_MINIMUM_FOR_TWO_EXPONENT  1

#define BITS_IN_BYTE 8

#define EXPONENT_OF_TWO_STATE 1

#define MESSAGE_ENTER_SIZE_OF_INPUT     "Enter size of input:\n"
#define MESSAGE_INVALID_SIZE            "Invalid size\n"
#define MESSAGE_ENTER_NUMBERS           "Enter numbers:\n"
#define MESSAGE_INVALID_NUMBER          "Invalid number\n"
#define MESSAGE_PRINT_NUMBRE_FORMAT     "The number %d is a power of 2: %d = 2^%d\n"
#define MESSAGE_PRINT_EXPONENT_TOTAL    "Total exponent sum is %d\n"

typedef struct numberProperties
{
    int     num;
    char    numberOfBits;
    char    exponent;
} NumberProperties, *pNumberProperties;

int main()
{
    int inputs;
    int size = 0;

    // Read size of numbers
    printf(MESSAGE_ENTER_SIZE_OF_INPUT);
    inputs = scanf("%d", &size);
    if (inputs !=  CHECK_INPUT_NUMBERS)
    {
        return ERROR_INVALID_ARGUMENT_ENTERED;
    }
    else if (size < CHECK_MINIMUM_INPUT_SIZE)
    {
        printf(MESSAGE_INVALID_SIZE);
        return ERROR_BAD_INPUT_SIZE;
    }

    // Allocate memory for numbers array of struct
    pNumberProperties pNumberPropertiesArray = malloc(sizeof(*pNumberPropertiesArray) * size);
    if (NULL == pNumberPropertiesArray) // if malloc failed
    {
        return ERROR_MALLOC_FAILED;
    }
    memset(pNumberPropertiesArray, 0, sizeof(*pNumberPropertiesArray) * size);

    // Read numbers and check if they are exponent of 2
    printf(MESSAGE_ENTER_NUMBERS);
    for (int i = 0; i < size; i++)
    {
        int num;
        inputs = scanf("%d", &num);
        if (inputs !=  CHECK_INPUT_NUMBERS) // if invalid argument entered by the user
        {
            printf(MESSAGE_INVALID_NUMBER);
            free(pNumberPropertiesArray);
            pNumberPropertiesArray = NULL;
            return ERROR_INVALID_ARGUMENT_ENTERED;
        }

        // Init current number struct in array
        pNumberPropertiesArray[i].num = num;
        pNumberPropertiesArray[i].numberOfBits = 0;
        pNumberPropertiesArray[i].exponent = 0;

        // check if number should be examined for 2 exponent
        if (num >= CHECK_MINIMUM_FOR_TWO_EXPONENT) // to prevent checkign of negative numbers and zeroes, they can't be exponent of 2
        {
            for (int j = 0; j < BITS_IN_BYTE * sizeof(int); j++) // check how many bits set to 1, if only 1 bit then this is an exponent of 2
            {
                if (num & (1 << j)) // check if bit j is set to 1
                {
                    pNumberPropertiesArray[i].numberOfBits++;
                    pNumberPropertiesArray[i].exponent = j; // will be valid only of numberOfBits will be equal to 1, otherwise not valid
                }
            }
        }
    }

    // Print numbers which are exponent of 2 and sum of exponents
    int sumExponent = 0;
    for (int i = 0; i < size; i++)
    {
        if (EXPONENT_OF_TWO_STATE == pNumberPropertiesArray[i].numberOfBits) // only if 1 bit is set to 1, the number is exponent of 2, othereise not
        {
            printf(MESSAGE_PRINT_NUMBRE_FORMAT, pNumberPropertiesArray[i].num, pNumberPropertiesArray[i].num, pNumberPropertiesArray[i].exponent);
            sumExponent += pNumberPropertiesArray[i].exponent;
        }
    }
    printf(MESSAGE_PRINT_EXPONENT_TOTAL, sumExponent);

    free(pNumberPropertiesArray);
    pNumberPropertiesArray = NULL;

    return ERROR_SUCCESS;
}
