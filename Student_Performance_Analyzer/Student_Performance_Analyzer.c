#include <stdio.h>

struct student
{
    int rollno;
    char name[30];
    int marks[3];
    int total_marks;
    int average_marks;
    char grade;
};

int find_total_marks(struct student s)
{
    return s.marks[0] + s.marks[1] + s.marks[2];
}

float find_average_marks(struct student s)
{
    return find_total_marks(s) / 3.0;
}

char find_grade(struct student s)
{
    float averageMarks = find_average_marks(s);
    if (averageMarks >= 85)
        return 'A';
    else if (averageMarks >= 70)
        return 'B';
    else if (averageMarks >= 50)
        return 'C';
    else if (averageMarks >= 35)
        return 'D';
    else
        return 'F';
}

void find_performance(char grade)
{

    int star = 0;
    switch (grade)
    {
    case 'A':
        star = 5;
        break;
    case 'B':
        star = 4;
        break;
    case 'C':
        star = 3;
        break;
    case 'D':
        star = 2;
        break;
    default:
        break;
    }

    if (grade == 'F')
        return;

    for (int i = 0; i < star; i++)
    {
        printf("* ");
    }
}

void ListOfRollNo(struct student s[], int index, int totalStudent)
{
    if (index == totalStudent)
        return;
    printf("%d ", s[index].rollno);
    ListOfRollNo(s, index + 1, totalStudent);
}

int main()
{
    struct student students[100];
    int n;
    printf("enter number of students: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        printf("enter details of student %d (Roll_Number Name Marks1 Marks2 Marks3) : ", i + 1);
        scanf("%d %s %d %d %d", &students[i].rollno, students[i].name, &students[i].marks[0], &students[i].marks[1], &students[i].marks[2]);
    }
    printf("\n\n************** Student Data *************\n\n");
    for (int i = 0; i < n; i++)
    {
        students[i].total_marks = find_total_marks(students[i]);
        students[i].average_marks = find_average_marks(students[i]);
        students[i].grade = find_grade(students[i]);

        printf("Roll no : %d\n", students[i].rollno);
        printf("Name : %s\n", students[i].name);
        printf("Total Marks : %d\n", students[i].total_marks);
        printf("Average Marks : %d\n", students[i].average_marks);
        printf("Grade : %c\n", students[i].grade);
        char student_grade = find_grade(students[i]);
        if (student_grade == 'F')
        {
            printf("\n\n");
            continue;
        }

        printf("Perfomance : ");
        find_performance(student_grade);

        printf("\n\n");
    }

    printf("\n\nList of Roll Numbers (via recursion) : ");
    ListOfRollNo(students, 0, n);
    printf("\n\n");

    return 0;
}