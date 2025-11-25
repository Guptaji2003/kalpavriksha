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

int getHashIndex(int key);
char *skipSpace(char *s);
Node *hashGet(int key);
void hashPut(int key, Node *node);
void hashRemove(int key);
void addToFront(Node *node);
void removeNode(Node *node);
void moveToFront(Node *node);
void evictLRU(void);
void freeCache(void);
void createCache(int capacity);
char *getValue(int key);
void putValue(int key, const char *value);

int getHashIndex(int key)
{
    int index = key % HASH_SIZE;
    if (index < 0)
    {
        index += HASH_SIZE;
    }
    return index;
}

char *skipSpace(char *alpha)
{
    while (*alpha && isspace((unsigned char)*alpha))
    {
        alpha++;
    }
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

    node->prev = node->next = NULL;
}

void moveToFront(Node *node)
{
    if (!node || cache->head == node)
    {
        return;
    }
    removeNode(node);
    addToFront(node);
}

Node *hashGet(int key)
{
    if (!cache)
    {
        return NULL;
    }
    int idx = getHashIndex(key);
    HashEntry *entry = cache->table[idx];
    while (entry)
    {
        if (entry->key == key)
            return entry->node;
        entry = entry->next;
    }
    return NULL;
}

void hashPut(int key, Node *node)
{
    if (!cache)
    {
        return;
    }
    int idx = getHashIndex(key);
    HashEntry *entry = malloc(sizeof(HashEntry));
    if (!entry)
    {
        fprintf(stderr, "Out of memory.\n");
        return;
    }
    entry->key = key;
    entry->node = node;
    entry->next = cache->table[idx];
    cache->table[idx] = entry;
}

void hashRemove(int key)
{
    if (!cache)
    {
        return;
    }
    int idx = getHashIndex(key);
    HashEntry *entry = cache->table[idx];
    HashEntry *prev = NULL;
    while (entry)
    {
        if (entry->key == key)
        {
            if (prev)
            {
                prev->next = entry->next;
            }
            else
            {
                cache->table[idx] = entry->next;
            }
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void evictLRU(void)
{
    if (!cache || !cache->tail)
    {
        return;
    }
    Node *lru = cache->tail;
    int key = lru->key;
    removeNode(lru);
    hashRemove(key);
    free(lru);
    if (cache->size > 0)
    {
        cache->size--;
    }
}

void createCache(int capacity)
{
    if (capacity <= 0)
    {
        fprintf(stderr, "capacity must be positive\n");
        return;
    }

    if (cache)
    {
        freeCache();
    }

    cache = malloc(sizeof(LRUCache));
    if (!cache)
    {
        fprintf(stderr, "Out of memory creating cache.\n");
        return;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        cache->table[i] = NULL;
    }

    printf("Cache created with capacity %d\n", capacity);
}

char *getValue(int key)
{
    if (!cache)
    {
        return NULL;
    }
    Node *node = hashGet(key);
    if (!node)
    {
        return NULL;
    }
    moveToFront(node);
    return node->value;
}

void putValue(int key, const char *value)
{
    if (!cache)
    {
        printf("Cache not created yet!\n");
        return;
    }
    if (!value)
    {
        return;
    }

    Node *node = hashGet(key);
    if (node)
    {
        strncpy(node->value, value, MAX_VALUE_LEN - 1);
        node->value[MAX_VALUE_LEN - 1] = '\0';
        moveToFront(node);
        return;
    }

    if (cache->size == cache->capacity)
    {
        evictLRU();
    }

    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        fprintf(stderr, "Out of memory creating node.\n");
        return;
    }
    newNode->key = key;
    strncpy(newNode->value, value, MAX_VALUE_LEN - 1);
    newNode->value[MAX_VALUE_LEN - 1] = '\0';
    newNode->prev = newNode->next = NULL;

    addToFront(newNode);
    hashPut(key, newNode);
    cache->size++;
}

void freeCache(void)
{
    if (!cache)
    {
        return;
    }

    Node *curr = cache->head;
    while (curr)
    {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }

    for (int i = 0; i < HASH_SIZE; ++i)
    {
        HashEntry *entry = cache->table[i];
        while (entry)
        {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
        cache->table[i] = NULL;
    }

    free(cache);
    cache = NULL;
}

int main(void)
{
    char buffer[256];
    char command[64];
    char *args;
    int key, capacity;
    char value[MAX_VALUE_LEN];
    char extra;
    printf("LRU Cache...\n");
    while (1)
    {
        printf("command >");
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            break;
        }

        args = skipSpace(buffer);
        if (*args == '\0')
        {
            continue;
        }

        if (sscanf(args, "%63s", command) != 1)
        {
            continue;
        }

        args = strstr(args, command);
        if (args)
            args += strlen(command);
        else
            args = buffer + strlen(buffer);
        args = skipSpace(args);

        if (strcmp(command, "createCache") == 0)
        {
            int matched = sscanf(args, "%d %c", &capacity, &extra);
            if (matched == 1 && capacity > 0)
            {
                createCache(capacity);
            }
            else
            {
                printf("createCache capacity should a positive Integer value\n");
            }
        }
        else if (strcmp(command, "put") == 0)
        {
            int matched = sscanf(args, "%d %99s %c", &key, value, &extra);
            if (matched == 2)
            {
                putValue(key, value);
            }
            else if (matched == 1)
            {
                printf("Usage: put <key> <value>\n");
            }
            else
            {
                printf("Usage: put <key> <value>\n");
            }
        }
        else if (strcmp(command, "get") == 0)
        {
            int matched = sscanf(args, "%d %c", &key, &extra);
            if (matched == 1)
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
            if (*args != '\0')
            {
                printf("exit command does not take argument\n");
            }
            else
            {
                freeCache();
                printf("Exiting...\n");
                break;
            }
        }
        else
        {
            printf("Invalid command...");
        }
    }

    return 0;
}
