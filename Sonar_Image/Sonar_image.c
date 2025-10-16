#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *matrixptr1, int *matrixptr2)

{

    int temporaryVariable = *matrixptr1;
    *matrixptr1 = *matrixptr2;
    *matrixptr2 =temporaryVariable;
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

int main()
{

    int lengthOfMatrix;
    printf("Enter the size of array : ");
    scanf("%d", &lengthOfMatrix);

    if (lengthOfMatrix < 2 || lengthOfMatrix > 10 || isdigit(lengthOfMatrix))
    {
        printf("Invalid input, n should be (2-10)\n");
        return 1;
    }

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
            scanf("%d", (ptr + row * lengthOfMatrix + column));
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
