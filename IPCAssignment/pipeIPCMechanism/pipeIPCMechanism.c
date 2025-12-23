#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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

int main()
{
    int p2c[2], c2p[2];
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));

    printf("Enter elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", arr+i);

    printf("Elements before sorting:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", *(arr+i));
    printf("\n");

    
    if( pipe(p2c) == -1 || pipe(c2p) == -1 )
    {
        perror("Pipe failed");
        free(arr);
        return 1;
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("Fork failed");
        free(arr);
        return 1;
    }

    if (pid == 0)
    {
        // CHILD
        close(p2c[1]); 
        close(c2p[0]); 

        read(p2c[0], arr, n * sizeof(int));
        sort(arr, n);
        write(c2p[1], arr, n * sizeof(int));

        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    }
    else
    {
        // PARENT
        close(p2c[0]);
        close(c2p[1]); 

        write(p2c[1], arr, n * sizeof(int));
        wait(NULL);
        read(c2p[0], arr, n * sizeof(int));

        printf("Sorted array in parent process:\n");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        close(p2c[1]);
        close(c2p[0]);
    }

    free(arr);
    return 0;
}
