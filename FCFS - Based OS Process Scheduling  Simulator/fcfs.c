#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 100

struct Process
{
    int id;
    char name[32];
    int arrival_time;
    int burst_time;
    int IO_start;
    int IO_duration;

    int remaining_burst;
    int cpu_executed;
    int remaining_io;
    int total_io_time;
    int finish_time;
    int is_killed;
    char state[16];
    struct Process *next;
};

struct hashnode
{
    struct Process *process;
    struct hashnode *next;
};

int hashfunction(int pid) { return pid % HASH_SIZE; }

void hashinsert(int pid, struct Process *process, struct hashnode *hashmap[])
{
    int idx = hashfunction(pid);
    struct hashnode *node = malloc(sizeof(struct hashnode));
    node->process = process;
    node->next = hashmap[idx];
    hashmap[idx] = node;
}

struct Process *hashsearch(int pid, struct hashnode *hashmap[])
{
    int idx = hashfunction(pid);
    struct hashnode *curr = hashmap[idx];
    while (curr)
    {
        if (curr->process->id == pid)
            return curr->process;
        curr = curr->next;
    }
    return NULL;
}

struct qnode
{
    struct Process *process;
    struct qnode *next;
};
struct Queue
{
    struct qnode *front, *rear;
};

struct Queue *createQueue()
{
    struct Queue *queue = malloc(sizeof(struct Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(struct Queue *queue, struct Process *process)
{
    struct qnode *node = malloc(sizeof(struct qnode));
    node->process = process;
    node->next = NULL;
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = node;
        return;
    }
    queue->rear->next = node;
    queue->rear = node;
}

struct Process *dequeue(struct Queue *queue)
{
    if (!queue->front)
        return NULL;
    struct qnode *temp = queue->front;
    struct Process *process = temp->process;
    queue->front = queue->front->next;
    if (!queue->front)
        queue->rear = NULL;
    free(temp);
    return process;
}

int isEmpty(struct Queue *queue) { return queue->front == NULL; }

int removeProcessFromQueue(struct Queue *queue, struct Process *target)
{
    struct qnode *curr = queue->front, *prev = NULL;
    while (curr)
    {
        if (curr->process == target)
        {
            if (prev == NULL)
                queue->front = curr->next;
            else
                prev->next = curr->next;
            if (queue->front == NULL)
                queue->rear = NULL;
            else if (prev && prev->next == NULL)
                queue->rear = prev;
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

struct KillCmd
{
    int pid;
    int time;
    struct KillCmd *next;
};
void insertKillCmd(struct KillCmd **head, int pid, int time)
{
    struct KillCmd *node = malloc(sizeof(struct KillCmd));
    node->pid = pid;
    node->time = time;
    node->next = *head;
    *head = node;
}

struct Process *parseProcessLine(char *line)
{
    struct Process *process = malloc(sizeof(struct Process));
    char nameStr[32], ioStartStr[16], ioDurStr[16];

    int pos = 0;
    int scanned = sscanf(line, "%31s %d %d %15s %15s %n",
                         nameStr, &process->id, &process->burst_time,
                         ioStartStr, ioDurStr, &pos);

    if (!(scanned == 3 || scanned == 5))
    {
        free(process);
        return NULL;
    }

    for (int i = pos; line[i] != '\0'; i++)
    {
        if (!isspace(line[i]))
        {
            free(process);
            return NULL;
        }
    }

    if (process->id <= 0)
    {
        free(process);
        return NULL;
    }

    if (process->burst_time <= 0)
    {
        free(process);
        return NULL;
    }

    strcpy(process->name, nameStr);

    if (scanned == 3)
    {
        process->IO_start = -1;
        process->IO_duration = 0;
    }
    else
    {

        if (strcmp(ioStartStr, "-") == 0)
            process->IO_start = -1;
        else
        {
            process->IO_start = atoi(ioStartStr);
            if (process->IO_start < 0)
            {
                free(process);
                return NULL;
            }
        }

        if (strcmp(ioDurStr, "-") == 0)
            process->IO_duration = 0;
        else
        {
            process->IO_duration = atoi(ioDurStr);
            if (process->IO_duration < 0)
            {
                free(process);
                return NULL;
            }
        }
    }

    process->arrival_time = 0;
    process->remaining_burst = process->burst_time;
    process->cpu_executed = 0;
    process->remaining_io = process->IO_duration;
    process->total_io_time = 0;
    process->finish_time = -1;
    process->is_killed = 0;
    strcpy(process->state, "READY");
    process->next = NULL;

    return process;
}

void simulate(struct Queue *ready, struct Queue *waiting, struct Queue *terminated,
              struct hashnode *hashmap[], struct KillCmd *killhead, int total_processes)
{
    int current_time = 0, finished = 0;
    struct Process *running = NULL;

    while (finished < total_processes)
    {
        for (struct KillCmd *killnode = killhead; killnode != NULL; killnode = killnode->next)
        {
            if (killnode->time == current_time)
            {
                struct Process *process = hashsearch(killnode->pid, hashmap);
                if (process && !process->is_killed && strcmp(process->state, "TERMINATED") != 0)
                {
                    process->is_killed = 1;
                    strcpy(process->state, "KILLED");
                    process->finish_time = current_time;
                    if (running == process)
                    {
                        running = NULL;
                    }
                    else
                    {
                        removeProcessFromQueue(ready, process);
                        removeProcessFromQueue(waiting, process);
                    }
                    enqueue(terminated, process);
                    finished++;
                }
            }
        }

        if (!isEmpty(waiting))
        {
            struct qnode *oldFront = waiting->front;
            struct qnode *oldRear = waiting->rear;

            waiting->front = NULL;
            waiting->rear = NULL;

            struct qnode *current = oldFront;

            while (current)
            {
                struct qnode *next = current->next;
                struct Process *process = current->process;
                free(current);

                if (!process->is_killed)
                {
                    if (process->remaining_io > 0)
                    {
                        process->remaining_io--;
                        process->total_io_time++;
                    }

                    if (process->remaining_io <= 0)
                    {
                        strcpy(process->state, "READY");
                        enqueue(ready, process);
                    }
                    else
                    {
                        enqueue(waiting, process);
                    }
                }

                current = next;
            }
        }

        if (running && !running->is_killed)
        {
            if (running->IO_start != -1 && running->cpu_executed == running->IO_start)
            {
                strcpy(running->state, "WAITING");
                running->remaining_io = running->IO_duration;
                enqueue(waiting, running);
                running = NULL;
            }
        }

        if (!running && !isEmpty(ready))
        {
            running = dequeue(ready);
            if (!running->is_killed)
                strcpy(running->state, "RUNNING");
            else
                running = NULL;
        }

        if (running && !running->is_killed)
        {
            running->remaining_burst--;
            running->cpu_executed++;
            if (running->remaining_burst <= 0)
            {
                running->finish_time = current_time + 1;
                strcpy(running->state, "TERMINATED");
                enqueue(terminated, running);
                finished++;
                running = NULL;
            }
        }

        current_time++;
    }
}

void printoutput(struct hashnode *hashmap[])
{
    printf("\nPID\tName\tCPU\tIO\tStatus\tTurnaround\tWaiting\n");
    for (int i = 0; i < HASH_SIZE; i++)
    {
        struct hashnode *curr = hashmap[i];
        while (curr)
        {
            struct Process *process = curr->process;
            int tat = (process->finish_time >= 0) ? (process->finish_time - process->arrival_time) : -1;
            int wt = (tat >= 0) ? (tat - process->burst_time) : -1;

            const char *status = process->is_killed ? "KILLED" : (strcmp(process->state, "TERMINATED") == 0 ? "OK" : process->state);
            int io_print = process->is_killed ? process->IO_duration : process->total_io_time;

            if (process->is_killed)
            {
                printf("%d\t%s\t%d\t%d\t%s\t-\t\t-\n", process->id, process->name, process->burst_time, io_print, status);
            }
            else
            {
                printf("%d\t%s\t%d\t%d\t%s\t%d\t\t%d\n", process->id, process->name, process->burst_time, io_print, status, tat, wt);
            }
            curr = curr->next;
        }
    }
}

void freeAll(struct Queue *ready, struct Queue *waiting, struct Queue *terminated,
             struct KillCmd *killhead, struct hashnode *hashmap[])
{
    while (killhead)
    {
        struct KillCmd *temp = killhead;
        killhead = killhead->next;
        free(temp);
    }

    if (terminated)
    {
        struct Process *process;
        while ((process = dequeue(terminated)) != NULL)
        {
            free(process);
        }
        free(terminated);
    }

    struct Queue *queues[2] = {ready, waiting};
    for (int i = 0; i < 2; i++)
    {
        struct Queue *queue = queues[i];
        if (!queue)
            continue;

        struct qnode *curr = queue->front;
        while (curr)
        {
            struct qnode *temp = curr;
            curr = curr->next;

            if (temp->process)
            {
                free(temp->process);
            }
            free(temp);
        }

        free(queue);
    }

    for (int i = 0; i < HASH_SIZE; i++)
    {
        struct hashnode *curr = hashmap[i];
        while (curr)
        {
            struct hashnode *temp = curr;
            curr = curr->next;
            free(temp);
        }
        hashmap[i] = NULL;
    }
}

int main()
{
    struct hashnode *hashmap[HASH_SIZE] = {0};
    struct Queue *ready = createQueue();
    struct Queue *waiting = createQueue();
    struct Queue *terminated = createQueue();
    struct KillCmd *killhead = NULL;

    char input[256];
    int total_processes = 0;
    printf("Enter processes and KILL commands. Type 'exit' to start simulation.\n");

    while (fgets(input, sizeof(input), stdin))
    {
        if (strcmp(input, "exit\n") == 0 || strcmp(input, "exit\r\n") == 0)
            break;
        char tmp[256];
        strcpy(tmp, input);
        char first[16];
        if (sscanf(tmp, "%15s", first) != 1)
            continue;
        if (strcasecmp(first, "kill") == 0)
        {
            int pid, t;
            if (sscanf(tmp, "kill %d %d", &pid, &t) == 2)
            {
                if (hashsearch(pid, hashmap) == NULL)
                {
                    printf("KILL ignored: PID %d does not exist.\n", pid);
                    continue;
                }
                insertKillCmd(&killhead, pid, t);
            }
            else
            {
                printf("Invalid KILL format. Use: kill <pid> <time>\n");
            }
        }
        else
        {
            struct Process *p = parseProcessLine(tmp);
            if (p)
            {
                if (hashsearch(p->id, hashmap) != NULL)
                {
                    printf("PID %d already exists. Process ignored.\n", p->id);
                    free(p);
                    continue;
                }
                else
                {
                    hashinsert(p->id, p, hashmap);
                    enqueue(ready, p);
                    total_processes++;
                }
            }
            else
            {
                printf("Invalid process format. Use: <name> <pid> <burst> [<IO_start> <IO_dur>]\n");
                continue;
            }
        }
    }

    if (total_processes == 0)
    {
        printf("\nNo processes entered. Exiting...\n");
        freeAll(ready, waiting, terminated, killhead, hashmap);
        return 0;
    }

    simulate(ready, waiting, terminated, hashmap, killhead, total_processes);
    printoutput(hashmap);
    freeAll(ready, waiting, terminated, killhead, hashmap);

    return 0;
}
