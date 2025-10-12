#include <stdio.h>
#include <stdlib.h>

struct students
{
    int rollno;
    char name[30];
    float marks[3];
    float total_marks;
    float average_marks;
    char grade;
};

float find_total_marks(struct students student)
{
    return student.marks[0] + student.marks[1] + student.marks[2];
}

float find_average_marks(struct students student)
{
    return find_total_marks(student) / 3.0;
}

char find_grade(struct students student)
{
    float averageMarks = find_average_marks(student);
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

void ListOfRollNo(struct students students[], int index, int totalStudent)
{
    if (index == totalStudent)
        return;
    printf("%d ", students[index].rollno);
    ListOfRollNo(students, index + 1, totalStudent);
}

int main()
{
    struct students students[100];
    int n;
    char input[100];

    while (1)
    {
        printf("enter number of students (1-100): ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = 0;

        int valid = 1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (!isdigit(input[i]))
            {
                valid = 0;
                break;
            }
        }

        if (!valid)
        {
            printf("invalid input, enter a positive integer\n");
            continue;
        }

        n = atoi(input);

        if (n < 1 || n > 100)
        {
            printf("number is  out of range, enter between 1 and 100\n");
            continue;
        }

        break;
    }

    for (int index = 0; index < n; index++)
    {
        printf("\nenter details like Roll_Number, Name, Marks1, Marks2, Marks3 for student %d: ", index + 1);

        if (scanf("%d %s %f %f %f", &students[index].rollno, students[index].name, &students[index].marks[0], &students[index].marks[1], &students[index].marks[2]) != 5)
        {
            printf("invalid input format please enter: Roll_Number Name Marks1 Marks2 Marks3\n");
            return 1;
        }

        if (students[index].rollno < 1 || students[index].rollno > 99999)
        {
            printf("error: Roll number must be between 1 and 99999\n");
            return 1;
        }

        for (int j = 0; j < 3; j++)
        {
            if (students[index].marks[j] < 0 || students[index].marks[j] > 100)
            {
                printf(" error: marks%d must be between 0 and 100\n", j + 1);
                return 1;
            }
        }
    }

    printf("\n\n************** Student Data *************\n\n");
    for (int index = 0; index < n; index++)
    {
        students[index].total_marks = find_total_marks(students[index]);
        students[index].average_marks = find_average_marks(students[index]);
        students[index].grade = find_grade(students[index]);

        printf("Roll no : %d\n", students[index].rollno);
        printf("Name : %s\n", students[index].name);
        printf("Total Marks : %.2f\n", students[index].total_marks);
        printf("Average Marks : %.2f\n", students[index].average_marks);
        printf("Grade : %c\n", students[index].grade);

        char student_grade = find_grade(students[index]);
        if (student_grade == 'F')
        {
            printf("\n\n");
            continue;
        }

        printf("Performance : ");
        find_performance(student_grade);
        printf("\n\n");
    }

    printf("List of Roll Numbers (via recursion): ");
    ListOfRollNo(students, 0, n);
    printf("\n\n");

    return 0;
}
