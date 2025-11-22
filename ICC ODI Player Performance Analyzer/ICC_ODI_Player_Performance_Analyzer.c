#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"
typedef struct playerNode
{
    int id;
    char *name;
    char *team;
    char *role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;

    struct playerNode *nextPlayer;
} playerNode;
typedef struct teamNode
{
    int teamID;
    const char *teamName;
    int totalPlayers;
    float totalStrikeRate;
    int battingPlayerCount;
    float avgBattingStrikeRate;

    struct playerNode *batsmenHead;
    struct playerNode *bowlerHead;
    struct playerNode *allRounderHead;
} teamNode;
int playerIDs[2000];
int playerIDCount = 0;
int playerIdSearch(int playerList[], int playerListSize, int playerId)
{
    int low = 0, high = playerListSize - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (playerList[mid] == playerId)
            return mid;
        else if (playerList[mid] < playerId)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}
int idExists(int playerId)
{
    return playerIdSearch(playerIDs, playerIDCount, playerId) != -1;
}
void insertPlayerID(int id)
{
    int i = playerIDCount - 1;
    while (i >= 0 && playerIDs[i] > id)
    {
        playerIDs[i + 1] = playerIDs[i];
        i--;
    }
    playerIDs[i + 1] = id;
    playerIDCount++;
}
void initializeTeams(teamNode teams[], int n);
void loadInitialData(teamNode teams[], int n);
void loadPlayerFromHeader(const Player p, teamNode teams[], int teamId);
void addPlayerToTeam(playerNode *player, int teamId, teamNode teams[]);
void addPlayerToRole(playerNode **head, playerNode *player);
void addNewPlayer(teamNode teams[]);
void printMenu();
void intValidateInRange(const char *prompt, int *out, int minv, int maxv);
void intValidateNonNeg(const char *prompt, int *out);
void floatValidateNonNeg(const char *prompt, float *out);
void roleSelection(playerNode *p);
void nameValidate(const char *prompt, char *buf, int maxLength);
void freeList(playerNode *head);
void freeAll(teamNode teams[], int n);
void flush_stdin(void);
void initializeTeams(teamNode Teams[], int teamCount)
{
    for (int i = 0; i < teamCount; i++)
    {
        Teams[i].teamID = i;
        Teams[i].teamName = teams[i];
        Teams[i].batsmenHead = NULL;
        Teams[i].bowlerHead = NULL;
        Teams[i].allRounderHead = NULL;
        Teams[i].totalPlayers = 0;
        Teams[i].battingPlayerCount = 0;
        Teams[i].totalStrikeRate = 0.0f;
        Teams[i].avgBattingStrikeRate = 0.0f;
    }
}
void loadPlayerFromHeader(const Player playerData, teamNode teams[], int teamId)
{
    playerNode *newplayer = malloc(sizeof(playerNode));
    if (!newplayer)
    {
        fprintf(stderr, "Memory alloc failed\n");
        return;
    }
    newplayer->id = playerData.id;
    newplayer->name = malloc(strlen(playerData.name) + 1);
    newplayer->team = malloc(strlen(playerData.team) + 1);
    newplayer->role = malloc(strlen(playerData.role) + 1);
    strcpy(newplayer->name, playerData.name);
    strcpy(newplayer->team, playerData.team);
    strcpy(newplayer->role, playerData.role);
    newplayer->totalRuns = playerData.totalRuns;
    newplayer->battingAverage = playerData.battingAverage;
    newplayer->strikeRate = playerData.strikeRate;
    newplayer->wickets = playerData.wickets;
    newplayer->economyRate = playerData.economyRate;

    if (strcmp(newplayer->role, "Batsman") == 0)
        newplayer->performanceIndex = (newplayer->battingAverage * newplayer->strikeRate) / 100.0f;
    else if (strcmp(newplayer->role, "Bowler") == 0)
        newplayer->performanceIndex = (newplayer->wickets * 2.0f) + (100.0f - newplayer->economyRate);
    else
        newplayer->performanceIndex = ((newplayer->battingAverage * newplayer->strikeRate) / 100.0f) + (newplayer->wickets * 2.0f);

    newplayer->nextPlayer = NULL;

    if (!idExists(newplayer->id))
        insertPlayerID(newplayer->id);

    addPlayerToTeam(newplayer, teamId, teams);
}
void loadInitialData(teamNode teams[], int teamCount)
{
    for (int playerIndex = 0; playerIndex < playerCount; playerIndex++)
    {
        int found = -1;
        for (int teamIndex = 0; teamIndex < teamCount; teamIndex++)
        {
            if (strcmp(players[playerIndex].team, teams[teamIndex].teamName) == 0)
            {
                found = teamIndex;
                break;
            }
        }
        if (found >= 0)
        {
            loadPlayerFromHeader(players[playerIndex], teams, found);
        }
        // else
        // {
        // }
    }
}
void flush_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
void intValidateInRange(const char *prompt, int *value, int minvalue, int maxvalue)
{
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d%c", value, &extra) != 2 || extra != '\n')
        {
            printf("Invalid input. Enter an integer.\n");
            flush_stdin();
            continue;
        }
        if (*value < minvalue || *value > maxvalue)
        {
            printf("Value out of range.\n");
            continue;
        }
        break;
    }
}
void intValidateNonNeg(const char *prompt, int *value)
{
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d%c", value, &extra) != 2 || extra != '\n')
        {
            printf("Invalid input.\n");
            flush_stdin();
            continue;
        }
        if (*value < 0)
        {
            printf("Value cannot be negative.\n");
            continue;
        }
        break;
    }
}
void floatValidateNonNeg(const char *prompt, float *value)
{
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%f%c", value, &extra) != 2 || extra != '\n')
        {
            printf("Invalid number.\n");
            flush_stdin();
            continue;
        }
        if (*value < 0)
        {
            printf("Cannot be negative.\n");
            continue;
        }
        break;
    }
}
void nameValidate(const char *prompt, char *buffer, int maxLength)
{
    while (1)
    {
        printf("%s: ", prompt);
        fflush(stdout);
        if (feof(stdin))
            clearerr(stdin);
        int c = getchar();
        if (c != '\n' && c != EOF)
        {
            ungetc(c, stdin);
        }

        if (!fgets(buffer, maxLength, stdin))
        {
            printf("Input error\n");
            continue;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == 0)
        {
            printf("Error: Name cannot be empty.\n");
            continue;
        }

        int onlySpaces = 1;
        for (size_t i = 0; i < len; ++i)
        {
            if (buffer[i] != ' ')
            {
                onlySpaces = 0;
                break;
            }
        }
        if (onlySpaces)
        {
            printf("Error: Name cannot contain only spaces.\n");
            continue;
        }

        if (len > 50)
        {
            printf("Error: Name must be between 1 and 50 characters.\n");
            continue;
        }

        int valid = 1;
        for (size_t i = 0; i < len; ++i)
        {
            char ch = buffer[i];
            if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == ' '))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf("Error: Name must contain only alphabets and spaces.\n");
            continue;
        }

        break;
    }
}
void roleSelection(playerNode *player)
{
    int rolenum;
    intValidateInRange("Select role (1-Batsman 2-Bowler 3-All-rounder): ", &rolenum, 1, 3);

    player->role = malloc(20);
    if (!player->role)
    {
        fprintf(stderr, "malloc failed\n");
        return;
    }

    switch (rolenum)
    {
    case 1:
        strcpy(player->role, "Batsman");
        break;
    case 2:
        strcpy(player->role, "Bowler");
        break;
    case 3:
        strcpy(player->role, "All-rounder");
        break;
    }
}
void addNewPlayer(teamNode teams[])
{
    int teamId;
    intValidateInRange("Enter Team ID (0-9): ", &teamId, 0, teamCount - 1);

    printf("Enter Player Details:\n");

    int tmpId;
    while (1)
    {
        intValidateInRange("Player ID (1-10000): ", &tmpId, 1, 10000);
        if (!idExists(tmpId))
            break;
        printf("ID already exists! Enter another.\n");
    }

    playerNode *player = malloc(sizeof(playerNode));
    if (!player)
    {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    memset(player, 0, sizeof(*player));
    player->id = tmpId;

    player->name = malloc(64);
    player->team = malloc(strlen(teams[teamId].teamName) + 1);
    if (!player->name || !player->team)
    {
        fprintf(stderr, "malloc failed\n");
        return;
    }

    nameValidate("Enter Name", player->name, 64);
    strcpy(player->team, teams[teamId].teamName);
    roleSelection(player);
    intValidateNonNeg("Total Runs: ", &player->totalRuns);
    floatValidateNonNeg("Batting Average: ", &player->battingAverage);
    floatValidateNonNeg("Strike Rate: ", &player->strikeRate);
    intValidateNonNeg("Wickets: ", &player->wickets);
    floatValidateNonNeg("Economy Rate: ", &player->economyRate);

    if (strcmp(player->role, "Batsman") == 0)
        player->performanceIndex = (player->battingAverage * player->strikeRate) / 100.0f;
    else if (strcmp(player->role, "Bowler") == 0)
        player->performanceIndex = (player->wickets * 2.0f) + (100.0f - player->economyRate);
    else
        player->performanceIndex = ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wickets * 2.0f);

    player->nextPlayer = NULL;

    insertPlayerID(player->id);

    addPlayerToTeam(player, teamId, teams);

    printf("Player added successfully in team %s.\n", teams[teamId].teamName);
}
void addPlayerToTeam(playerNode *player, int teamId, teamNode teams[])
{
    teams[teamId].totalPlayers++;

    if (strcmp(player->role, "Batsman") == 0 || strcmp(player->role, "All-rounder") == 0)
    {
        teams[teamId].totalStrikeRate += player->strikeRate;
        teams[teamId].battingPlayerCount++;
        teams[teamId].avgBattingStrikeRate = teams[teamId].totalStrikeRate / teams[teamId].battingPlayerCount;
    }

    if (strcmp(player->role, "Bowler") == 0)
        addPlayerToRole(&teams[teamId].bowlerHead, player);
    else if (strcmp(player->role, "Batsman") == 0)
        addPlayerToRole(&teams[teamId].batsmenHead, player);
    else
        addPlayerToRole(&teams[teamId].allRounderHead, player);
}
void addPlayerToRole(playerNode **head, playerNode *player)
{
    if (*head == NULL || player->performanceIndex > (*head)->performanceIndex)
    {
        player->nextPlayer = *head;
        *head = player;
        return;
    }

    playerNode *cur = *head;
    while (cur->nextPlayer && cur->nextPlayer->performanceIndex > player->performanceIndex)
        cur = cur->nextPlayer;

    player->nextPlayer = cur->nextPlayer;
    cur->nextPlayer = player;
}
void DisplayAllPlayersOfASpecificTeam(teamNode teams[])
{
    int teamId;
    intValidateInRange("Enter Team ID (0-9): ", &teamId, 0, teamCount - 1);

    printf("Players of team %s:\n", teams[teamId].teamName);
    printf("======================================================================\n");
    printf("ID\tName\t\tRole\t\tRuns\tAvg\tSR\tWkts\tER\tPerfIndex\n");
    printf("======================================================================\n");

    playerNode *p = teams[teamId].batsmenHead;
    while (p != NULL)
    {
        printf("%d\t%-15s\t%-10s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               p->id, p->name, p->role, p->totalRuns, p->battingAverage,
               p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
        p = p->nextPlayer;
    }

    p = teams[teamId].allRounderHead;
    while (p != NULL)
    {
        printf("%d\t%-15s\t%-10s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               p->id, p->name, p->role, p->totalRuns, p->battingAverage,
               p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
        p = p->nextPlayer;
    }

    p = teams[teamId].bowlerHead;
    while (p != NULL)
    {
        printf("%d\t%-15s\t%-10s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               p->id, p->name, p->role, p->totalRuns, p->battingAverage,
               p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
        p = p->nextPlayer;
    }

    printf("=====================================================================\n");
    printf("Total Players: %d\n", teams[teamId].totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", teams[teamId].avgBattingStrikeRate);
}
void DisplayTeamsbyAverageBattingStrikeRate(teamNode teams[], int n)
{
    int *idx = malloc(sizeof(int) * n);
    if (!idx)
    {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    for (int i = 0; i < n; ++i)
        idx[i] = i;

    for (int i = 0; i < n - 1; ++i)
        for (int j = i + 1; j < n; ++j)
            if (teams[idx[j]].avgBattingStrikeRate > teams[idx[i]].avgBattingStrikeRate)
            {
                int tmp = idx[i];
                idx[i] = idx[j];
                idx[j] = tmp;
            }

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("======================================================\n");
    printf("Team ID\tTeam Name\t\tAvg Bat SR\tTotal Players\n");
    printf("======================================================\n");

    for (int k = 0; k < n; ++k)
    {
        int i = idx[k];
        printf("%d\t%-15s\t%.2f\t\t%d\n",
               teams[i].teamID, teams[i].teamName,
               teams[i].avgBattingStrikeRate, teams[i].battingPlayerCount);
    }
    printf("========================================\n");
    free(idx);
}
void DisplayTopKPlayersofaSpecificTeamofspecificrole(teamNode teams[])
{
    int teamId;
    intValidateInRange("Enter Team ID (0-9): ", &teamId, 0, teamCount - 1);
    int role;
    intValidateInRange("Role (1-Batsman, 2-Bowler, 3-All-rounder): ", &role, 1, 3);
    int numberOfPlayers;
    intValidateNonNeg("Enter number of players: ", &numberOfPlayers);

    printf("\nTop %d ", numberOfPlayers);
    if (role == 1)
        printf("Batsmen ");
    else if (role == 2)
        printf("Bowlers ");
    else
        printf("All-rounders ");
    printf("of Team %s\n", teams[teamId].teamName);

    printf("======================================================\n");
    printf("ID\tName\tTeam\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("======================================================\n");

    playerNode *temp;
    if (role == 1)
        temp = teams[teamId].batsmenHead;
    else if (role == 2)
        temp = teams[teamId].bowlerHead;
    else
        temp = teams[teamId].allRounderHead;

    int printed = 0;
    while (numberOfPlayers-- > 0 && temp != NULL)
    {
        printf("%d\t%-15s\t%-10s\t%-10s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               temp->id, temp->name, teams[teamId].teamName, temp->role,
               temp->totalRuns, temp->battingAverage, temp->strikeRate,
               temp->wickets, temp->economyRate, temp->performanceIndex);
        temp = temp->nextPlayer;
        printed++;
    }
    if (printed == 0)
        printf("No players available in this role for the selected team.\n");

    printf("======================================================\n");
}
typedef struct HeapNode
{
    playerNode *player;
    int teamIndex;
} HeapNode;
void swapHeap(HeapNode *a, HeapNode *b)
{
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}
void heapifyDown(HeapNode heap[], int size, int idx)
{
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < size && heap[left].player->performanceIndex > heap[largest].player->performanceIndex)
        largest = left;
    if (right < size && heap[right].player->performanceIndex > heap[largest].player->performanceIndex)
        largest = right;

    if (largest != idx)
    {
        swapHeap(&heap[idx], &heap[largest]);
        heapifyDown(heap, size, largest);
    }
}
void heapifyUp(HeapNode heap[], int idx)
{
    while (idx > 0)
    {
        int parent = (idx - 1) / 2;
        if (heap[idx].player->performanceIndex <= heap[parent].player->performanceIndex)
            break;
        swapHeap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}
void DisplayAllPlayersAcrossAllTeamsOfspecificRole(teamNode teams[], int teamCount)
{
    int role;
    intValidateInRange("Role (1-Batsman, 2-Bowler, 3-All-rounder):", &role, 1, 3);

    printf("\nPlayers in Descending Performance Index:\n");
    printf("======================================================\n");
    printf("ID\tName\tTeam\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerfIndex\n");
    printf("======================================================\n");

    HeapNode *heap = malloc(sizeof(HeapNode) * teamCount);
    if (!heap)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    int heapSize = 0;

    for (int i = 0; i < teamCount; ++i)
    {
        playerNode *temp = NULL;
        if (role == 1)
            temp = teams[i].batsmenHead;
        else if (role == 2)
            temp = teams[i].bowlerHead;
        else
            temp = teams[i].allRounderHead;

        if (temp != NULL)
        {
            heap[heapSize].player = temp;
            heap[heapSize].teamIndex = i;
            heapifyUp(heap, heapSize);
            heapSize++;
        }
    }

    while (heapSize > 0)
    {
        playerNode *top = heap[0].player;
        int idx = heap[0].teamIndex;

        printf("%d\t%-15s\t%-10s\t%-10s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               top->id, top->name, teams[idx].teamName, top->role,
               top->totalRuns, top->battingAverage, top->strikeRate,
               top->wickets, top->economyRate, top->performanceIndex);

        if (top->nextPlayer != NULL)
            heap[0].player = top->nextPlayer;
        else
        {
            heap[0] = heap[heapSize - 1];
            heapSize--;
        }
        heapifyDown(heap, heapSize, 0);
    }

    free(heap);
    printf("======================================================\n");
}
void freeList(playerNode *head)
{
    while (head)
    {
        playerNode *next = head->nextPlayer;
        free(head->name);
        free(head->team);
        free(head->role);
        free(head);
        head = next;
    }
}
void freeAll(teamNode teams[], int teamCount)
{
    for (int index = 0; index < teamCount; ++index)
    {
        freeList(teams[index].batsmenHead);
        freeList(teams[index].bowlerHead);
        freeList(teams[index].allRounderHead);
    }
}
void printMenu()
{
    printf("========================================\n");
    printf("  ICC ODI Player Performance Analyzer\n");
    printf("========================================\n");
    printf("1. Add Player to team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of specific role Across All Teams\n");
    printf("6. Exit\n");
}
int main(void)
{
    teamNode teamsList[teamCount];
    initializeTeams(teamsList, teamCount);
    loadInitialData(teamsList, teamCount);

    int choice;
    while (1)
    {
        printMenu();
        intValidateInRange("Enter your choice: ", &choice, 1, 6);

        switch (choice)
        {
        case 1:
            addNewPlayer(teamsList);
            break;
        case 2:
            DisplayAllPlayersOfASpecificTeam(teamsList);
            break;
        case 3:
            DisplayTeamsbyAverageBattingStrikeRate(teamsList, teamCount);
            break;
        case 4:
            DisplayTopKPlayersofaSpecificTeamofspecificrole(teamsList);
            break;
        case 5:
            DisplayAllPlayersAcrossAllTeamsOfspecificRole(teamsList, teamCount);
            break;
        case 6:
            freeAll(teamsList, teamCount);
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
