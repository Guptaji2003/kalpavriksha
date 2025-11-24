#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 1009
#define MAX_VALUE_LEN 100

typedef struct Node
{
    int key;
    char value[MAX_VALUE_LEN];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct HashEntry
{
    int key;
    Node *node;
    struct HashEntry *next;
} HashEntry;

typedef struct
{
    int capacity;
    int size;
    HashEntry *table[HASH_SIZE];
    Node *head;
    Node *tail;
} LRUCache;

LRUCache *cache = NULL;

int getHashIndex(int key)
{
    int index = key % HASH_SIZE;
    return (index < 0) ? index + HASH_SIZE : index;
}

char *skipSpace(char *alpha)
{
    while (*alpha && isspace((unsigned char)*alpha))
        alpha++;
    return alpha;
}

void addToFront(Node *node)
{
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head)
    {
        cache->head->prev = node;
    }
    cache->head = node;
    if (cache->tail == NULL)
    {
        cache->tail = node;
    }
}

void removeNode(Node *node)
{
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        cache->head = node->next;
    }
    if (node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        cache->tail = node->prev;
    }
}

void moveToFront(Node *node)
{
    if (cache->head == node)
    {
        return;
    }
    removeNode(node);
    addToFront(node);
}

Node *hashGet(int key)
{
    int index = getHashIndex(key);
    HashEntry *entry = cache->table[index];

    while (entry)
    {
        if (entry->key == key)
        {
            return entry->node;
        }
        entry = entry->next;
    }
    return NULL;
}

void hashPut(int key, Node *node)
{
    int index = getHashIndex(key);
    HashEntry *newEntry = malloc(sizeof(HashEntry));
    if (newEntry == NULL)
    {
        printf("Memory not allocated");
        return;
    }
    newEntry->key = key;
    newEntry->node = node;
    newEntry->next = cache->table[index];
    cache->table[index] = newEntry;
}

void hashRemove(int key)
{
    int index = getHashIndex(key);
    HashEntry *entry = cache->table[index], *prev = NULL;

    while (entry)
    {
        if (entry->key == key)
        {
            if (prev)
                prev->next = entry->next;
            else
                cache->table[index] = entry->next;
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void evictLRU()
{
    if (!cache->tail)
    {
        return;
    }
    Node *lru = cache->tail;
    int key = lru->key;

    removeNode(lru);
    hashRemove(key);
    free(lru);
    cache->size--;
}

void createCache(int capacity)
{
    if (cache)
    {
        free(cache);
        cache = NULL;
    }

    cache = malloc(sizeof(LRUCache));
    if (cache == NULL)
    {
        printf("Memory not allocated");
        return;
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int i = 0; i < HASH_SIZE; i++)
        cache->table[i] = NULL;

    printf("Cache created with capacity %d\n", capacity);
}

char *getValue(int key)
{
    if (!cache)
        return NULL;

    Node *node = hashGet(key);
    if (!node)
        return NULL;

    moveToFront(node);
    return node->value;
}

void putValue(int key, char *value)
{
    if (!cache)
    {
        printf("Cache not created yet!\n");
        return;
    }

    Node *node = hashGet(key);

    if (node)
    {
        strcpy(node->value, value);
        moveToFront(node);
        return;
    }

    if (cache->size == cache->capacity)
        evictLRU();

    Node *newNode = malloc(sizeof(Node));
     if (newNode == NULL)
    {
        printf("Memory not allocated");
        return;
    }
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = NULL;
    newNode->next = NULL;

    addToFront(newNode);
    hashPut(key, newNode);
    cache->size++;
}

void freeCache()
{
    if (!cache)
        return;

    Node *curr = cache->head;
    while (curr)
    {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }

    for (int i = 0; i < HASH_SIZE; i++)
    {
        HashEntry *h = cache->table[i];
        while (h)
        {
            HashEntry *tmp = h;
            h = h->next;
            free(tmp);
        }
    }

    free(cache);
    cache = NULL;
    printf("Cache freed.\n");
}

int main()
{
    char input[200], command[50], value[MAX_VALUE_LEN], extra;
    int key, capacity;

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            break;

        char *line = skipSpace(input);
        if (sscanf(line, "%s", command) != 1)
            continue;

        line = strstr(line, command) + strlen(command);
        line = skipSpace(line);

        if (strcmp(command, "createCache") == 0)
        {
            int match = sscanf(line, "%d %c", &capacity, &extra);

            if (match == 1 && capacity > 0)
            {
                createCache(capacity);
            }
            else
            {
                printf("Usage: createCache <positive int>\n");
            }
        }

        else if (strcmp(command, "put") == 0)
        {
            int match = sscanf(line, "%d %s %c", &key, value, &extra);

            if (match == 2)
                putValue(key, value);
            else
                printf("Usage: put <key> <value>\n");
        }

        else if (strcmp(command, "get") == 0)
        {
            int match = sscanf(line, "%d %c", &key, &extra);

            if (match == 1)
            {
                char *res = getValue(key);
                printf("%s\n", res ? res : "NULL");
            }
            else
            {
                printf("Usage: get <key>\n");
            }
        }

        else if (strcmp(command, "exit") == 0)
        {
            if (*line != '\0')
                printf("'exit' does not take arguments.\n");
            else
            {
                freeCache();
                break;
            }
        }
        else
        {
            printf("Unknown command!\n");
        }
    }

    return 0;
}
