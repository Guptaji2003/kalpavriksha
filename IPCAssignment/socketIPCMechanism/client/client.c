#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void printMenu()
{
    printf("\n--- ATM MENU ---\n");
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    printf("3. Display Balance\n");
    printf("4. Exit\n");
    printf("Enter choice: ");
}

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to ATM Server\n");

    int choice;
    while (1)
    {
        printMenu();
        scanf("%d", &choice);

        send(sock, &choice, sizeof(choice), 0);

        if (choice == 4)
        {
            printf("Exiting...\n");
            break;
        }

        int amount = 0;
        if (choice == 1 || choice == 2)
        {
            printf("Enter amount: ");
            scanf("%d", &amount);
            send(sock, &amount, sizeof(amount), 0);
        }

        int balance;
        if (recv(sock, &balance, sizeof(balance), 0) < 0)
        {
            perror("Server disconnected");
            break;
        }
        printf("Current Balance: %d\n", balance);
    }

    close(sock);
    return 0;
}
