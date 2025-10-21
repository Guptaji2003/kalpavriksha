#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void swap(int *matrixptr1, int *matrixptr2)

{

    int temporaryVariable = *matrixptr1;
    *matrixptr1 = *matrixptr2;
    *matrixptr2 = temporaryVariable;
}

void reverse(int *matrixptr1, int *matrixptr2)
{
    while (matrixptr1 < matrixptr2)
    {
        swap(matrixptr1, matrixptr2);
        matrixptr1++;
        matrixptr2--;
    }
}

void smoothFilter(int row, int column, int lengthOfMatrix, int *ptr)
{
    int average = 0;

    int sum = 0, count = 0;
    for (int filterRowIndex = -1; filterRowIndex <= 1; filterRowIndex++)
    {
        int filterRow = row + filterRowIndex;
        if (filterRow < 0 || filterRow >= lengthOfMatrix)
            continue;
        for (int filterColumnIndex = -1; filterColumnIndex <= 1; filterColumnIndex++)
        {
            int filterColumn = column + filterColumnIndex;
            if (filterColumn < 0 || filterColumn >= lengthOfMatrix)
                continue;

            sum += (*(ptr + filterRow * lengthOfMatrix + filterColumn)) & 0xFF;
            count++;
        }
    }
    average = sum / count;
    *(ptr + row * lengthOfMatrix + column) |= (average << 8);
}

void displayMatrix(int lengthOfMatrix, int *ptr)
{
    for (int row = 0; row < lengthOfMatrix; row++)
    {
        for (int column = 0; column < lengthOfMatrix; column++)
        {
            printf("%3d ", *(ptr + row * lengthOfMatrix + column));
        }
        printf("\n");
    }
}

int isEmptyInput(const char *input)
{
    return input[0] == '\n' || input[0] == '\0';
}

int isSpacesOnlyInput(const char *input)
{
    int i = 0;
    while (input[i] != '\0' && input[i] != '\n')
    {
        if (!isspace((unsigned char)input[i]))
            return 0;
        i++;
    }
    return 1;
}

void matrixLengthValidation(int *lengthOfMatrix)
{
    char ch;
    char input[100];

    do
    {
        printf("Enter a number between (2-10): ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Error in reading the input, try again\n");
            continue;
        }

        if (isEmptyInput(input))
        {
            printf("Empty input, Please enter a number\n");
            continue;
        }

        if (isSpacesOnlyInput(input))
        {
            printf("input contains only spaces, Please enter a valid number\n");
            continue;
        }

        if (sscanf(input, "%d%c", lengthOfMatrix, &ch) != 2 || ch != '\n')
        {
            printf("You entered an invalid input, please try again\n");
            continue;
        }

        if (*lengthOfMatrix < 2 || *lengthOfMatrix > 10)
        {
            printf("You entered invalid input, please enter between (2-10)\n");
            continue;
        }

        printf("\nYou entered a valid input: %d\n\n", *lengthOfMatrix);
        break;

    } while (1);
}

int main()
{

    int lengthOfMatrix;
    matrixLengthValidation(&lengthOfMatrix);

    int *ptr = (int *)malloc(lengthOfMatrix * lengthOfMatrix * sizeof(int));
    if (ptr == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    srand(time(NULL));

    for (int row = 0; row < lengthOfMatrix; row++)
    {
        for (int column = 0; column < lengthOfMatrix; column++)
        {
            *(ptr + row * lengthOfMatrix + column) = rand() % 256;
        }
    }

    printf("Array elements are:\n");
    displayMatrix(lengthOfMatrix, ptr);

    for (int row = 0; row < lengthOfMatrix; row++)
    {
        for (int column = row + 1; column < lengthOfMatrix; column++)
        {
            swap(ptr + row * lengthOfMatrix + column, ptr + column * lengthOfMatrix + row);
        }
    }

    for (int row = 0; row < lengthOfMatrix; row++)
    {
        reverse(ptr + row * lengthOfMatrix, ptr + row * lengthOfMatrix + lengthOfMatrix - 1);
    }

    printf("\nArray elements after rotation are:\n");
    displayMatrix(lengthOfMatrix, ptr);

    for (int row = 0; row < lengthOfMatrix; row++)
    {
        for (int column = 0; column < lengthOfMatrix; column++)
        {
            smoothFilter(row, column, lengthOfMatrix, ptr);
        }
    }

    for (int row = 0; row < lengthOfMatrix; row++)
    {
        for (int column = 0; column < lengthOfMatrix; column++)
        {
            *(ptr + row * lengthOfMatrix + column) = (*(ptr + row * lengthOfMatrix + column) >> 8) & 0xFF;
        }
    }

    printf("\nArray elements after smoothing are:\n");
    displayMatrix(lengthOfMatrix, ptr);

    free(ptr);
    return 0;
}
