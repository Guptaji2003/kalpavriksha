#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *DATA_FILE = "userdata.txt";

typedef struct {
    int user_id;
    char user_name[30];
    int user_age;
} User;

void addUser() {
    FILE *file = fopen(DATA_FILE, "a");
    if (file == NULL) {
        printf("error: unable to open %s for writing.\n", DATA_FILE);
        return;
    }

    User user;
    printf("enter user id: ");
    if (scanf("%d", &user.user_id) != 1) {
        printf("invalid input for user id.\n");
        fclose(file);
        return;
    }

    printf("enter user name: ");
    scanf("%s", user.user_name);

    printf("enter user age: ");
    if (scanf("%d", &user.user_age) != 1) {
        printf("invalid input for user age.\n");
        fclose(file);
        return;
    }

    fprintf(file, "%d %s %d\n", user.user_id, user.user_name, user.user_age);
    fclose(file);
    printf("user added successfully.\n");
}

void showUsers() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("here no data file found, add some user first.\n");
        return;
    }

    User user;
    printf("\nuser records-\n");
    while (fscanf(file, "%d %s %d", &user.user_id, user.user_name, &user.user_age) == 3) {
        printf("ID: %d | Name: %s | Age: %d\n", user.user_id, user.user_name, user.user_age);
    }

    fclose(file);
}

void updateUser() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("error: cannot open this %s for reading.\n", DATA_FILE);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("error:- cannot create temporary file.\n");
        fclose(file);
        return;
    }

    int userId, found = 0;
    printf("enter user id for update: ");
    if (scanf("%d", &userId) != 1) {
        printf("invalid input for id.\n");
        fclose(file);
        fclose(temp);
        return;
    }

    User user;
    while (fscanf(file, "%d %s %d", &user.user_id, user.user_name, &user.user_age) == 3) {
        if (user.user_id == userId) {
            printf("enter new name: ");
            scanf("%s", user.user_name);
            printf("enter new age: ");
            scanf("%d", &user.user_age);
            found = 1;
        }
        fprintf(temp, "%d %s %d\n", user.user_id, user.user_name, user.user_age);
    }

    fclose(file);
    fclose(temp);
    remove(DATA_FILE);
    rename("temp.txt", DATA_FILE);

    if (found)
        printf("user updated successfully.\n");
    else
        printf("no user found of this id.\n");
}

void deleteUser() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("error: cannot open %s for reading.\n", DATA_FILE);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("error: cannot create temporary file.\n");
        fclose(file);
        return;
    }

    int userId, found = 0;
    printf("enter user id to delete the user: ");
    if (scanf("%d", &userId) != 1) {
        printf("invalid input for id.\n");
        fclose(file);
        fclose(temp);
        return;
    }

    User user;
    while (fscanf(file, "%d %s %d", &user.user_id, user.user_name, &user.user_age) == 3) {
        if (user.user_id == userId) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %d\n", user.user_id, user.user_name, user.user_age);
    }

    fclose(file);
    fclose(temp);
    remove(DATA_FILE);
    rename("temp.txt", DATA_FILE);

    if (found)
        printf("user deleted successfully.\n");
    else
        printf("no user found of this id.\n");
}

int main() {
    int choice;
    while (1) {
        printf("\nuser management options-\n");
        printf("1. Add user\n");
        printf("2. Show all users\n");
        printf("3. Update user\n");
        printf("4. delete user\n");
        printf("5. exit\n");
        printf("enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("invalid input, please enter a number between 1-5.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }

        switch (choice) {
            case 1: addUser(); break;
            case 2: showUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: 
                printf("program exit--\n");
                return 0;
            default:
                printf("please enter some valid choice.\n");
        }
    }
}
