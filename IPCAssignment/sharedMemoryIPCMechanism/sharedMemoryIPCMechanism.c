#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define MAX 100

struct shm
{
    int n;
    int arr[MAX];
};

void sort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (*(arr + j) > *(arr + j + 1))
            {
                int temp = *(arr + j);
                *(arr + j) = *(arr + j + 1);
                *(arr + j + 1) = temp;
            }
        }
    }
}

void printArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", *(arr + i));
    }
    printf("\n");
}

int main()
{
    int shmid = shmget(1234, sizeof(struct shm), 0666 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("shmget failed");
        return 1;
    }

    struct shm *data = (struct shm *)shmat(shmid, NULL, 0);
    if (data == (void *)-1)
    {
        perror("shmat failed");
        return 1;
    }

    printf("Enter number of elements: ");
    scanf("%d", &data->n);

    printf("Enter elements:\n");
    for (int i = 0; i < data->n; i++)
    {
        scanf("%d", data->arr + i);
    }

    printf("Before sorting:\n");
    printArray(data->arr, data->n);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }
    if (pid == 0)
    {
        // Child process
        sort(data->arr, data->n);
        exit(0);
    }
    else
    {
        // Parent process
        wait(NULL);

        printf("After sorting (parent reads shared memory):\n");
        printArray(data->arr, data->n);
    }

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
