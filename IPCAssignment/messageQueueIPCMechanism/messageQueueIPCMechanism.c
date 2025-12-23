#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX 100

struct msg
{
    long type;
    int n;
    int arr[MAX];
};

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

int main()
{
    int msgid = msgget(1234, 0666 | IPC_CREAT);
    if (msgid < 0)
    {
        perror("msgget failed");
        return 1;
    }

    struct msg m;
    m.type = 1;

    printf("Enter number of elements: ");
    scanf("%d", &m.n);

    printf("Enter elements:\n");
    for (int i = 0; i < m.n; i++)
    {
        scanf("%d", m.arr + i);
    }

    printf("You Entered:\n");
    printArray(m.arr, m.n);

    int result = msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0);
    if (msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0) == -1)
    {
        perror("msgsnd failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0)
    {
        // Child
        struct msg r;
        if (msgrcv(msgid, &r, sizeof(r) - sizeof(long), 1, 0) == -1)
        {
            perror("msgrcv failed");
            exit(1);
        }

        sort(r.arr, r.n);

        if (msgsnd(msgid, &r, sizeof(r) - sizeof(long), 0) == -1)
        {
            perror("msgsnd failed");
            exit(1);
        }
        exit(0);
    }
    else
    {
        wait(NULL);

        struct msg r;
        if (msgrcv(msgid, &r, sizeof(r) - sizeof(long), 1, 0) == -1)
        {
            perror("msgrcv failed");
            return 1;
        }

        printf("Sorted array:\n");
        printArray(r.arr, r.n);
    }
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
