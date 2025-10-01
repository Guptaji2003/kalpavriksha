#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define file "users.txt"

struct usr {
    int id;
    char name[30];
    int age;
};

void adduser() {
    FILE *f=fopen(file,"a");
    if(f==NULL) {
        printf("file not open\n");
        return;
    }
    struct usr u;
    printf("enter id: ");
    scanf("%d",&u.id);
    printf("enter nam: ");
    scanf("%s",u.name);
    printf("enter ag: ");
    scanf("%d",&u.age);
    fprintf(f,"%d %s %d\n",u.id,u.name,u.age);
    fclose(f);
    printf("user add sucess\n");
}

void showall() {
    FILE *f=fopen(file,"r");
    if(f==NULL) {
        printf("no data presnt\n");
        return;
    }
    struct usr u;
    printf("---all usrs---\n");
    while(fscanf(f,"%d %s %d",&u.id,u.name,&u.age)==3) {
        printf("%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(f);
}

void updateuser() {
    FILE *f=fopen(file,"r");
    if(f==NULL) {
        printf("no file present\n");
        return;
    }
    FILE *t=fopen("temp.txt","w");
    struct usr u;
    int id,ok=0;
    printf("id to update: ");
    scanf("%d",&id);
    while(fscanf(f,"%d %s %d",&u.id,u.name,&u.age)==3) {
        if(u.id==id) {
            printf("new name: ");
            scanf("%s",u.name);
            printf("new age: ");
            scanf("%d",&u.age);
            ok=1;
        }
        fprintf(t,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(f); fclose(t);
    remove(file);
    rename("temp.txt",file);
    if(ok) printf("update done\n");
    else printf("id not found\n");
}

void deleteuser() {
    FILE *f=fopen(file,"r");
    if(f==NULL) {
        printf("no file present\n");
        return;
    }
    FILE *t=fopen("temp.txt","w");
    struct usr u;
    int id,ok=0;
    printf("id to delet: ");
    scanf("%d",&id);

    while(fscanf(f, "%d %s   %d", &u.id, u.name,&u.age)==3) {
        if(u.id==id) {
             ok=1; continue;
             }
        fprintf(t,"%d %s %d\n",u.id, u.name, u.age);
    } 
    fclose(f); fclose(t);
    remove(file);
    rename("temp.txt",file);
    if(ok) printf("deleted\n");
    else printf("id not found");
}

int main() {
    int ch;
    while(1) {
        printf("1 adduser \n2 showall\n 3 update\n 4 delete\n 5 exit\n");
        scanf("%d",&ch);
        if(ch==1) adduser();
        else if(ch==2) showall();
        else if(ch==3) updateuser();
        else if(ch==4) deleteuser();
        else if(ch==5) break;
        else printf("wrong choice");
    }
    return 0;
}
