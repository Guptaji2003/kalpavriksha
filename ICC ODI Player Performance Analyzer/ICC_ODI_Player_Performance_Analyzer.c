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
void initializeTeams(teamNode teams[], int n);
void addplayertoteam(playerNode *player, int teamId, teamNode teams[]);
playerNode *addplayerToLrole(playerNode *playerhead, playerNode *player);
void createPlayer(playerNode *p, teamNode teams[]);
void printMenu();
void intValidateInRange(const char *prompt, int *out, int minv, int maxv);
void intValidateNonNeg(const char *prompt, int *out);
void floatValidateNonNeg(const char *prompt, float *out);
void roleSelection(playerNode *p);
void freeList(playerNode *head);
void freeAll(teamNode teams[], int n);

void initializeTeams(teamNode Teams[], int teamCount)
{
    for (int index = 0; index < teamCount; index++)
    {
        Teams[index].teamID = index;
        Teams[index].teamName = teams[index];
        Teams[index].batsmenHead = NULL;
        Teams[index].bowlerHead = NULL;
        Teams[index].allRounderHead = NULL;
        Teams[index].totalPlayers = 0;
        Teams[index].battingPlayerCount = 0;
        Teams[index].totalStrikeRate = 0.0f;
        Teams[index].avgBattingStrikeRate = 0.0f;
    }
}
void loadPlayer(Player player, teamNode teams[], int teamId)
{
    playerNode *newplayer = malloc(sizeof(playerNode));
    newplayer->id = player.id;
    newplayer->name = malloc(64);
    newplayer->team = malloc(64);
    newplayer->role = malloc(64);
    strcpy(newplayer->name, player.name);
    strcpy(newplayer->team, player.team);
    strcpy(newplayer->role, player.role);
    newplayer->totalRuns = player.totalRuns;
    newplayer->battingAverage = player.battingAverage;
    newplayer->strikeRate = player.strikeRate;
    newplayer->wickets = player.wickets;
    newplayer->economyRate = player.economyRate;

    if (strcmp(newplayer->role, "Batsman") == 0)
        newplayer->performanceIndex = (newplayer->battingAverage * newplayer->strikeRate) / 100.0f;

    else if (strcmp(newplayer->role, "Bowler") == 0)
        newplayer->performanceIndex = (newplayer->wickets * 2.0f) + (100.0f - newplayer->economyRate);

    else
        newplayer->performanceIndex = ((newplayer->battingAverage * newplayer->strikeRate) / 100.0f) + (newplayer->wickets * 2.0f);

    newplayer->nextPlayer = NULL;

    addplayertoteam(newplayer, teamId, teams);
}

void loadInitialData(teamNode teams[])
{
    for (int index = 0; index < playerCount; index++)
    {
        if (strcmp(players[index].team, "Afghanistan") == 0)
        {
            loadPlayer(players[index], teams, 0);
        }
        else if (strcmp(players[index].team, "Australia") == 0)
        {
            loadPlayer(players[index], teams, 1);
        }
        else if (strcmp(players[index].team, "Bangladesh") == 0)
        {
            loadPlayer(players[index], teams, 2);
        }
        else if (strcmp(players[index].team, "England") == 0)
        {
            loadPlayer(players[index], teams, 3);
        }
        else if (strcmp(players[index].team, "India") == 0)
        {
            loadPlayer(players[index], teams, 4);
        }
        else if (strcmp(players[index].team, "New Zealand") == 0)
        {
            loadPlayer(players[index], teams, 5);
        }
        else if (strcmp(players[index].team, "Pakistan") == 0)
        {
            loadPlayer(players[index], teams, 6);
        }
        else if (strcmp(players[index].team, "South Africa") == 0)
        {
            loadPlayer(players[index], teams, 7);
        }
        else if (strcmp(players[index].team, "Sri Lanka") == 0)
        {
            loadPlayer(players[index], teams, 8);
        }
        else if (strcmp(players[index].team, "West Indies") == 0)
        {
            loadPlayer(players[index], teams, 9);
        }
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
void intValidateInRange(const char *prompt, int *value, int minvalue, int maxvalue)
{
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d%c", value, &extra) != 2 || extra != '\n')
        {
            printf("Invalid input. Enter an integer.\n");
            while (getchar() != '\n')
                ;
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
void nameValidate(const char *prompt, char *name, int maxLength)
{
    while (1)
    {
        printf("%s : ", prompt);

        fgets(name, maxLength, stdin);

        size_t length = strlen(name);
        if (length > 0 && name[length - 1] == '\n')
            name[length - 1] = '\0';

        if (strlen(name) == 0)
        {
            printf("Error: Name cannot be empty.\n");
            continue;
        }

        int onlySpaces = 1;
        for (int i = 0; name[i] != '\0'; i++)
        {
            if (name[i] != ' ')
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

        if (strlen(name) > 50)
        {
            printf("Error: Name must be between 1 to 50 characters.\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; name[i] != '\0'; i++)
        {
            if (!((name[i] >= 'A' && name[i] <= 'Z') ||
                  (name[i] >= 'a' && name[i] <= 'z') ||
                  (name[i] == ' ')))
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

void intValidateNonNeg(const char *prompt, int *value)
{
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d%c", value, &extra) != 2 || extra != '\n')
        {
            printf("Invalid input.\n");
            while (getchar() != '\n')
                ;
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
            while (getchar() != '\n')
                ;
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
void roleSelection(playerNode *player)
{
    int roleIndex;
    intValidateInRange("Select role (1-Batsman 2-Bowler 3-All-rounder): ", &roleIndex, 1, 3);

    player->role = malloc(20);

    switch (roleIndex)
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
void createPlayer(playerNode *player, teamNode teams[])
{
    int teamId;
    intValidateInRange("Enter Team ID (0-9): ", &teamId, 0, 9);

    printf("Enter Player Details:\n");

    intValidateInRange("Player ID (1-1000): ", &player->id, 1, 1000);

    player->name = malloc(64);
    player->team = malloc(64);

    nameValidate("Enter Name", player->name, 50);
    nameValidate("Enter team Name", player->team, 50);

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

    addplayertoteam(player, teamId, teams);

    printf("Player added successfully in team %s.\n", teams[teamId].teamName);
}
void addplayertoteam(playerNode *player, int teamId, teamNode teams[])
{
    teams[teamId].totalPlayers++;

    if (strcmp(player->role, "Batsman") == 0 || strcmp(player->role, "All-rounder") == 0)
    {
        teams[teamId].totalStrikeRate += player->strikeRate;
        teams[teamId].battingPlayerCount++;
        teams[teamId].avgBattingStrikeRate =
            teams[teamId].totalStrikeRate / teams[teamId].battingPlayerCount;
    }

    if (strcmp(player->role, "Bowler") == 0)
        teams[teamId].bowlerHead = addplayerToLrole(teams[teamId].bowlerHead, player);

    else if (strcmp(player->role, "Batsman") == 0)
        teams[teamId].batsmenHead = addplayerToLrole(teams[teamId].batsmenHead, player);

    else
        teams[teamId].allRounderHead = addplayerToLrole(teams[teamId].allRounderHead, player);
}
playerNode *addplayerToLrole(playerNode *head, playerNode *player)
{
    if (head == NULL || player->performanceIndex > head->performanceIndex)
    {
        player->nextPlayer = head;
        return player;
    }

    playerNode *curr = head;
    while (curr->nextPlayer &&
           curr->nextPlayer->performanceIndex > player->performanceIndex)
    {
        curr = curr->nextPlayer;
    }

    player->nextPlayer = curr->nextPlayer;
    curr->nextPlayer = player;

    return head;
}
void DisplayAllPlayersOfASpecificTeam(teamNode teams[])
{
    int teamId;
    intValidateInRange("Enter Team ID (0-9): ", &teamId, 0, 9);

    printf("Players of team %s:\n", teams[teamId].teamName);
    printf("======================================================================\n");
    printf("ID\tName\t\tRole\t\tRuns\tAvg\tSR\tWkts\tER\tPerfIndex\n");
    printf("======================================================================\n");

    playerNode *player = teams[teamId].batsmenHead;
    while (player != NULL)
    {
        printf("%d\t%s\t\t%s\t\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               player->id, player->name, player->role, player->totalRuns, player->battingAverage,
               player->strikeRate, player->wickets, player->economyRate, player->performanceIndex);
        player = player->nextPlayer;
    }

    player = teams[teamId].allRounderHead;
    while (player != NULL)
    {
        printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               player->id, player->name, player->role, player->totalRuns, player->battingAverage,
               player->strikeRate, player->wickets, player->economyRate, player->performanceIndex);
        player = player->nextPlayer;
    }

    player = teams[teamId].bowlerHead;
    while (player != NULL)
    {
        printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               player->id, player->name, player->role, player->totalRuns, player->battingAverage,
               player->strikeRate, player->wickets, player->economyRate, player->performanceIndex);
        player = player->nextPlayer;
    }

    printf("=====================================================================\n");
    printf("Total Players: %d\n", teams[teamId].totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", teams[teamId].avgBattingStrikeRate);
}
void DisplayTeamsbyAverageBattingStrikeRate(teamNode teams[], int teamCount)
{
    for (int i = 0; i < teamCount - 1; i++)
    {
        for (int j = i + 1; j < teamCount; j++)
        {
            if (teams[j].avgBattingStrikeRate > teams[i].avgBattingStrikeRate)
            {
                teamNode temp = teams[i];
                teams[i] = teams[j];
                teams[j] = temp;
            }
        }
    }

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("======================================================\n");
    printf("Team ID\tTeam Name\t\tAvg Bat SR\tTotal Players\n");
    printf("======================================================\n");

    for (int index = 0; index < teamCount; index++)
    {
        printf("%d\t%-15s\t%.2f\t\t%d\n",
               teams[index].teamID,
               teams[index].teamName,
               teams[index].avgBattingStrikeRate,
               teams[index].battingPlayerCount);
    }

    printf("========================================\n");
}

void DisplayTopKPlayersofaSpecificTeamofspecificrole(teamNode teams[])
{
    int teamId;
    intValidateInRange("Enter Team ID (0-9): ", &teamId, 0, 9);
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

    int availableplayer = 0;
    while (numberOfPlayers-- > 0 && temp != NULL)
    {
        availableplayer++;
        printf("%d\t%s\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               temp->id, temp->name, teams[teamId].teamName, temp->role,
               temp->totalRuns, temp->battingAverage, temp->strikeRate,
               temp->wickets, temp->economyRate, temp->performanceIndex);

        temp = temp->nextPlayer;
    }
    printf("\nOnly %d players available in this role.\n", availableplayer);
    printf("======================================================\n");
}

typedef struct HeapNode
{
    playerNode *player;
    int teamIndex;
} HeapNode;
void swap(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
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
        swap(&heap[idx], &heap[largest]);
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
        swap(&heap[idx], &heap[parent]);
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

    HeapNode *heap = (HeapNode *)malloc(sizeof(HeapNode) * teamCount);
    int heapSize = 0;

    for (int i = 0; i < teamCount; i++)
    {
        playerNode *temp;

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

        printf("%d\t%s\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
               top->id, top->name, teams[idx].teamName, top->role,
               top->totalRuns, top->battingAverage, top->strikeRate,
               top->wickets, top->economyRate, top->performanceIndex);

        if (top->nextPlayer != NULL)
        {
            heap[0].player = top->nextPlayer;
        }
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
    for (int index = 0; index < teamCount; index++)
    {
        freeList(teams[index].batsmenHead);
        freeList(teams[index].bowlerHead);
        freeList(teams[index].allRounderHead);
    }
}
int main(void)
{
    teamNode teams[teamCount];
    initializeTeams(teams, teamCount);
    loadInitialData(teams);

    playerNode Player;
    int choice;

    while (1)
    {
        printMenu();
        intValidateInRange("Enter your choice: ", &choice, 1, 6);

        switch (choice)
        {
        case 1:
            createPlayer(&Player, teams);
            break;
        case 2:
            DisplayAllPlayersOfASpecificTeam(teams);
            break;
        case 3:
            DisplayTeamsbyAverageBattingStrikeRate(teams, 10);
            break;
        case 4:
            DisplayTopKPlayersofaSpecificTeamofspecificrole(teams);
            break;
        case 5:
            DisplayAllPlayersAcrossAllTeamsOfspecificRole(teams, 10);
            break;
        case 6:
            freeAll(teams, 10);
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
