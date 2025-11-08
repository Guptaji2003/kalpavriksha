#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TOTAL_BLOCKS 1024
#define BLOCK_SIZE 512

char virtualDisk[TOTAL_BLOCKS][BLOCK_SIZE];

struct freeBlock
{
    int data;
    struct freeBlock *prev;
    struct freeBlock *next;
};

struct fileNode
{
    char name[50];
    int isFile;
    struct fileNode *parent;
    struct fileNode *next;
    struct fileNode *child;
    int blockPointer[10];
};
void convertEscapes(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] == '\\' && str[i + 1] == 'n') {
            str[j++] = '\n';  
            i += 2;
        } else {
            str[j++] = str[i++];
        }
    }
    str[j] = '\0';
}

void freeAllNodes(struct fileNode *dir)
{
    if (dir == NULL)
        return;

    struct fileNode *start = dir;
    struct fileNode *trav = dir;

    do
    {
        struct fileNode *next = trav->next;

        if (trav->isFile == 0 && trav->child != NULL)
        {
            freeAllNodes(trav->child);
        }

        free(trav);

        trav = next;
    } while (trav != start);
}
void addFreeBlock(struct freeBlock **head, struct freeBlock **tail, int data)
{
    struct freeBlock *temp = malloc(sizeof(struct freeBlock));
    temp->data = data;
    temp->next = NULL;
    temp->prev = NULL;
    if (*head == NULL)
    {
        *head = temp;
        *tail = temp;
        return;
    }
    (*tail)->next = temp;
    temp->prev = *tail;
    *tail = (*tail)->next;
}
void mkdir_cmd(struct fileNode **cwd, char *name)
{

    struct fileNode *check = (*cwd)->child;
    if (check != NULL)
    {
        do
        {
            if (strcmp(check->name, name) == 0 && check->isFile == 0)
            {
                printf("Name already exists in current directory.\n");
                return;
            }
            check = check->next;
        } while (check != (*cwd)->child);
    }
    struct fileNode *temp = malloc(sizeof(struct fileNode));
    strncpy(temp->name, name, 49);
    temp->name[49] = '\0';
    temp->parent = *cwd;
    temp->next = NULL;
    temp->child = NULL;
    temp->isFile = 0;

    if ((*cwd)->child == NULL)
    {
        (*cwd)->child = temp;
        temp->next = temp;
        printf("Directory '%s' created successfully.\n", name);
        return;
    }
    struct fileNode *ptr = (*cwd)->child;
    struct fileNode *trav = ptr;
    while (trav->next != ptr)
    {
        trav = trav->next;
    }
    temp->next = ptr;
    trav->next = temp;
    printf("Directory '%s' created successfully.\n", name);
}
void create_cmd(struct fileNode **cwd, char *name)
{
    struct fileNode *check = (*cwd)->child;
    if (check != NULL)
    {
        do
        {
            if (strcmp(check->name, name) == 0 && check->isFile == 1)
            {
                printf("Name already exists in current directory.\n");
                return;
            }
            check = check->next;
        } while (check != (*cwd)->child);
    }

    struct fileNode *temp = malloc(sizeof(struct fileNode));
    strncpy(temp->name, name, 49);
    temp->name[49] = '\0';
    temp->parent = *cwd;
    temp->next = NULL;
    temp->child = NULL;
    temp->isFile = 1;

    for (int i = 0; i < 10; i++)
    {
        temp->blockPointer[i] = -1;
    }
    if ((*cwd)->child == NULL)
    {
        (*cwd)->child = temp;
        temp->next = temp;
        printf("File '%s' created successfully.\n", name);
        return;
    }
    struct fileNode *ptr = (*cwd)->child;
    struct fileNode *trav = ptr;
    while (trav->next != ptr)
    {
        trav = trav->next;
    }
    temp->next = ptr;
    trav->next = temp;
    printf("File '%s' created successfully.\n", name);
}
void write_cmd(struct fileNode **cwd, char *name, char *data, struct freeBlock **head)
{
    struct fileNode *temp = (*cwd)->child;
    if (temp == NULL)
    {
        printf("File not found.\n");
        return;
    }
    struct fileNode *trav = (*cwd)->child;
    do
    {
        int j = 0;
        while (j < 10 && trav->blockPointer[j] != -1)
        {
            j++;
        }
        if (strcmp(trav->name, name) == 0 && trav->isFile == 1)
        {
            int blocks_count = (strlen(data) + 511) / 512;
            for (int i = 0; i < blocks_count; i++)
            {
                if (*head == NULL)
                {
                    printf("No free blocks available!\n");
                    return;
                }

                struct freeBlock *ptr = *head;
                *head = (*head)->next;
                ptr->next = NULL;
                ptr->prev = NULL;
                if (*head != NULL)
                {
                    (*head)->prev = NULL;
                }
                int start = i * 512;
                strncpy(virtualDisk[ptr->data], data + start, 512);
                virtualDisk[ptr->data][511] = '\0';
                trav->blockPointer[j++] = ptr->data;
            }
            printf("Data written successfully (size=%d bytes).\n", strlen(data));
            return;
        }
        trav = trav->next;
    } while (trav != temp);
    printf("file not found.\n");
}
void read_cmd(struct fileNode **cwd, char *name)
{
    struct fileNode *temp = (*cwd)->child;
    if (temp == NULL)
    {
        printf("File not found.\n");
        return;
    }
    struct fileNode *trav = (*cwd)->child;
    do
    {
        if (strcmp(trav->name, name) == 0 && trav->isFile == 1)
        {
            int j = 0;
            while (trav->blockPointer[j] != -1)
            {
                printf("%s", virtualDisk[trav->blockPointer[j]]);
                j++;
            }
            if (j == 0)
            {
                printf("file is empty.");
            }
            printf("\n");
            return;
        }
        trav = trav->next;
    } while (trav != temp);
    printf("file not found.\n");
}
void delete_cmd(struct freeBlock **head, struct freeBlock **tail, struct fileNode **cwd, char *name)
{
    struct fileNode *temp = (*cwd)->child;
    if (temp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    struct fileNode *headNode = (*cwd)->child;
    struct fileNode *trav = headNode;
    struct fileNode *prev = NULL;

    do
    {
        if (strcmp(trav->name, name) == 0 && trav->isFile == 1)
        {
            int j = 0;
            while (trav->blockPointer[j] != -1)
            {
                strcpy(virtualDisk[trav->blockPointer[j]], "");
                addFreeBlock(head, tail, trav->blockPointer[j]);
                trav->blockPointer[j] = -1;
                j++;
            }

            if (trav->next == trav)
            {
                (*cwd)->child = NULL;
            }
            else if (prev == NULL)
            {
                struct fileNode *last = headNode;
                while (last->next != headNode)
                    last = last->next;
                last->next = trav->next;
                (*cwd)->child = trav->next;
            }
            else
            {
                prev->next = trav->next;
            }

            free(trav);
            printf("File deleted successfully.\n");
            return;
        }
        else if (strcmp(trav->name, name) == 0 && trav->isFile == 0)
        {
            printf("Error: Cannot delete.\n");
            return;
        }

        prev = trav;
        trav = trav->next;
    } while (trav != headNode);

    printf("File not found.\n");
}
void rmdir_cmd(struct fileNode **cwd, char *name)
{
    struct fileNode *temp = (*cwd)->child;

    if (temp == NULL)
    {
        printf("Directory not found.\n");
        return;
    }

    struct fileNode *head = (*cwd)->child;
    struct fileNode *trav = head;
    struct fileNode *prev = NULL;

    if (head->next == head)
    {
        if (strcmp(head->name, name) == 0 && head->isFile == 0)
        {
            if (head->child != NULL)
            {
                printf("Directory is not empty.\n");
                return;
            }
            free(head);
            (*cwd)->child = NULL;
            printf("Directory removed successfully.\n");
            return;
        }
        else
        {
            printf("Directory not found.\n");
            return;
        }
    }

    do
    {
        if (strcmp(trav->name, name) == 0 && trav->isFile == 0)
        {
            if (trav->child != NULL)
            {
                printf("Directory is not empty.\n");
                return;
            }

            if (prev == NULL)
            {
                struct fileNode *last = head;
                while (last->next != head)
                    last = last->next;
                last->next = trav->next;
                (*cwd)->child = trav->next;
            }
            else
            {
                prev->next = trav->next;
            }

            free(trav);
            trav = NULL;
            printf("Directory removed successfully.\n");
            return;
        }

        prev = trav;
        trav = trav->next;

    } while (trav != head);

    printf("Directory not found.\n");
}
void ls_cmd(struct fileNode *cwd)
{
    struct fileNode *temp = cwd->child;
    if (temp == NULL)
    {
        printf("(empty)\n");
        return;
    }
    struct fileNode *trav = cwd->child;
    do
    {
        if (trav->isFile == 0)
        {
            printf("%s/\n", trav->name);
        }
        else
        {
            printf("%s\n", trav->name);
        }
        trav = trav->next;
    } while (trav != temp);
}
void cd_cmd(struct fileNode **cwd, char *name)
{
    if (strcmp(name, "..") == 0)
    {
        if ((*cwd)->parent == NULL)
        {
            printf("Error: Cannot go back now....\n");
            return;
        }
        *cwd = (*cwd)->parent;
        if ((*cwd)->parent == NULL)
        {
            printf("Moved to %s\n", (*cwd)->name);
            return;
        }
        printf("Moved to /%s\n", (*cwd)->name);
        return;
    }
    struct fileNode *temp = (*cwd)->child;
    if (temp == NULL)
    {
        printf("Directory not found.\n");
        return;
    }
    struct fileNode *trav = (*cwd)->child;
    do
    {
        if (strcmp(trav->name, name) == 0 && trav->isFile == 0)
        {
            *cwd = trav;
            printf("Moved to /%s\n", trav->name);
            return;
        }
        trav = trav->next;
    } while (trav != temp);
    printf("Directory not found.\n");
}
void pwd_cmd(struct fileNode *root, struct fileNode *cwd)
{
    if (root == cwd)
    {
        printf("/");
        return;
    }
    char path[500] = "";
    struct fileNode *temp = cwd;
    while (temp != root)
    {
        char word[100];
        sprintf(word, "/%s", temp->name);
        char newPath[500];
        strcpy(newPath, word);
        strcat(newPath, path);
        strcpy(path, newPath);
        temp = temp->parent;
    }
    printf("%s", path);
    return;
}
void df_cmd(struct freeBlock *head)
{
    struct freeBlock *temp = head;
    int count = 0;
    while (temp != NULL)
    {
        temp = temp->next;
        count++;
    }
    int total_blocks = TOTAL_BLOCKS;
    int free_blocks = count;
    int used_blocks = total_blocks - free_blocks;
    float disk_usage = ((float)used_blocks / total_blocks) * 100;
    printf("Total Blocks : %d\n", total_blocks);
    printf("Used Blocks : %d\n", used_blocks);
    printf("Free Blocks : %d\n", free_blocks);
    printf("Disk Usage : %.2f%%\n", disk_usage);
}
void exit_cmd(struct freeBlock **head, struct fileNode **root)
{
    struct freeBlock *temp = *head;
    while (temp != NULL)
    {
        struct freeBlock *next = temp->next;
        free(temp);
        temp = next;
    }

    if ((*root)->child != NULL)
    {
        freeAllNodes((*root)->child);
    }
    free(*root);

    printf("\nExiting Compact VFS...\nAll memory released successfully.\n");
}

int main()
{
    struct fileNode *root = malloc(sizeof(struct fileNode));
    strcpy(root->name, "/");
    root->parent = NULL;
    root->next = NULL;
    root->child = NULL;
    root->isFile = 0;

    struct fileNode *cwd = root;

    struct freeBlock *head = NULL;
    struct freeBlock *tail = NULL;
    for (int i = 0; i < 1024; i++)
    {
        addFreeBlock(&head, &tail, i);
    }

    char command[100], word[100];

    printf("$ ./vfs \nCompact VFS - ready. Type 'exit' to quit.\n");

    while (1)
    {
        pwd_cmd(root, cwd);
        printf(">");
        scanf("%s", command);

        if (strcmp(command, "mkdir") == 0)
        {
            scanf("%s", word);
            mkdir_cmd(&cwd, word);
        }
        else if (strcmp(command, "create") == 0)
        {
            scanf("%s", word);
            create_cmd(&cwd, word);
        }
        else if (strcmp(command, "write") == 0)
        {
            char data[1000];
            scanf("%s", word);
            getchar();
            fgets(data, sizeof(data), stdin);

            data[strcspn(data, "\n")] = '\0';
            convertEscapes(data);

            if (data[0] == '"' && data[strlen(data) - 1] == '"')
            {
                memmove(data, data + 1, strlen(data) - 1);
                data[strlen(data) - 2] = '\0';
            }
            else
            {
                printf("Error: Data must be inside double quotes (\").\n");
                continue;
            }

            write_cmd(&cwd, word, data, &head);
        }

        else if (strcmp(command, "read") == 0)
        {
            scanf("%s", word);
            read_cmd(&cwd, word);
        }
        else if (strcmp(command, "delete") == 0)
        {
            scanf("%s", word);
            delete_cmd(&head, &tail, &cwd, word);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            scanf("%s", word);
            rmdir_cmd(&cwd, word);
        }
        else if (strcmp(command, "ls") == 0)
        {
            ls_cmd(cwd);
        }
        else if (strcmp(command, "cd") == 0)
        {
            scanf("%s", word);
            cd_cmd(&cwd, word);
        }
        else if (strcmp(command, "pwd") == 0)
        {
            printf("Path: ");
            pwd_cmd(root, cwd);
            printf("\n");
        }
        else if (strcmp(command, "df") == 0)
        {
            df_cmd(head);
        }
        else if (strcmp(command, "exit") == 0)
        {
            exit_cmd(&head, &root);
            break;
        }
        else
        {
            printf("Invalid command! Try again.\n");
        }
    }

    return 0;
}
