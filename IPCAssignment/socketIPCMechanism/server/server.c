#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define DB_FILE "../resource/accountDB.txt"

pthread_mutex_t lock;

int updateBalance(int choice, int amount)
{
    FILE *fp = fopen(DB_FILE, "r+");
    if (!fp)
    {
        perror("File open error");
        return -1;
    }

    int balance;
    fscanf(fp, "%d", &balance);

    if (choice == 1)
    {
        if (amount <= balance)
            balance -= amount;
    }
    else if (choice == 2) 
    {
        balance += amount;
    }

    rewind(fp);
    fprintf(fp, "%d", balance);
    fclose(fp);

    return balance;
}

void *clientHandler(void *arg)
{
    int client_sock = *(int *)arg;
    free(arg);

    int choice;

    while (1)
    {
        if (recv(client_sock, &choice, sizeof(choice), 0) <= 0)
            break;

        if (choice == 4)
            break;

        int amount = 0;
        if (choice == 1 || choice == 2)
        {
            recv(client_sock, &amount, sizeof(amount), 0);
        }

        pthread_mutex_lock(&lock);

        int balance = updateBalance(choice, amount);
        if( balance == -1 )
        {
            pthread_mutex_unlock(&lock);
            close(client_sock);
            break;
        }

        pthread_mutex_unlock(&lock);

        send(client_sock, &balance, sizeof(balance), 0);
    }

    close(client_sock);
    return NULL;
}

int main()
{
    FILE *fp = fopen(DB_FILE, "r");
    if (fp == NULL)
    {
        fp = fopen(DB_FILE, "w");
        fprintf(fp, "1000");
    }
    fclose(fp);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *)&server, sizeof(server));
    listen(server_sock, 5);

    pthread_mutex_init(&lock, NULL);

    printf("ATM Server running on port %d...\n", PORT);

    while (1)
    {
        int client_sock = accept(server_sock, NULL, NULL);
        if (client_sock < 0)
            continue;

        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;

        pthread_create(&tid, NULL, clientHandler, pclient);
        pthread_detach(tid);
    }

    close(server_sock);
    pthread_mutex_destroy(&lock);
    return 0;
}
