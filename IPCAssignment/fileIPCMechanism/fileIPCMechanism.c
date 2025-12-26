#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "data.txt"

void sort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void writeToFile(const char *filename, int *arr, int n)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("File open failed");
        exit(1);
    }

    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d ", arr[i]);
    }
    fclose(fp);
}

void readFromFile(const char *filename, int *arr, int *n)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("File open failed");
        exit(1);
    }

    fscanf(fp, "%d", n);
    for (int i = 0; i < *n; i++)
    {
        fscanf(fp, "%d", &arr[i]);
    }
    fclose(fp);
}

int main()
{
    int n;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("Invalid input\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    printf("Enter elements:\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    writeToFile(FILE_NAME, arr, n);

    printf("\nArray before sorting:\n");
    printArray(arr, n);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        free(arr);
        return 1;
    }

    if (pid == 0)
    {
        readFromFile(FILE_NAME, arr, &n);

        sort(arr, n);

        writeToFile(FILE_NAME, arr, n);

        exit(0);
    }
    else
    {
        wait(NULL);

        readFromFile(FILE_NAME, arr, &n);

        printf("\nArray after sorting:\n");
        printArray(arr, n);
    }

    free(arr);
    remove(FILE_NAME);
    return 0;
}
