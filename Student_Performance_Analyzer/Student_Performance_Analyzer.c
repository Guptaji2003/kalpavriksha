#include <stdio.h>

struct students
{
    int rollno;
    char name[30];
    int marks[3];
    int total_marks;
    int average_marks;
    char grade;
};

int find_total_marks(struct students student)
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
    printf("enter number of students: ");
    scanf("%d", &n);
    if (n > 100 || n < 1)
    {
        printf("invalid number of students, please enter a number between 1 and 100 \n");
        return 1;
    }
    for (int index = 0; index < n; index++)
    {
        printf("enter details of student %d (Roll_Number Name Marks1 Marks2 Marks3) : ", index + 1);
        scanf("%d %s %d %d %d", &students[index].rollno, students[index].name, &students[index].marks[0], &students[index].marks[1], &students[index].marks[2]);
        if (students[index].marks[0] < 0 || students[index].marks[0] > 100 || students[index].marks[1] < 0 || students[index].marks[1] > 100 || students[index].marks[2] < 0 || students[index].marks[2] > 100)
        {
            printf("invalid marks, please enter marks between 0 and 100 \n");
            return 1;
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
        printf("Total Marks : %d\n", students[index].total_marks);
        printf("Average Marks : %d\n", students[index].average_marks);
        printf("Grade : %c\n", students[index].grade);
        char student_grade = find_grade(students[index]);
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