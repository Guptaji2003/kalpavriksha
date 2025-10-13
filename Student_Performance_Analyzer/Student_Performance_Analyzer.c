#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct
{
    int rollNumber;
    char studentName[50];
    double subjectMark1;
    double subjectMark2;
    double subjectMark3;
    double totalMarks;
    double averageMarks;
    char grade;
} Student;

void printRollNumbers(Student studentList[], int currentIndex, int totalStudents)
{
    if (currentIndex == totalStudents)
        return;

    printf("%d ", studentList[currentIndex].rollNumber);
    printRollNumbers(studentList, currentIndex + 1, totalStudents);
}

int isValidIntegerInput(char *inputString)
{
    if (inputString[0] == '\0')
        return 0;

    for (int index = 0; inputString[index] != '\0'; index++)
    {
        if (!isdigit(inputString[index]))
            return 0;
    }
    return 1;
}

void sortStudents_ByRollNumber(Student studentList[], int totalStudents)
{
    for (int outerIndex = 0; outerIndex < totalStudents - 1; outerIndex++)
    {for (int innerIndex = 0; innerIndex < totalStudents - outerIndex - 1; innerIndex++)
        {
            if (studentList[innerIndex].rollNumber > studentList[innerIndex + 1].rollNumber){
                Student tempStudent = studentList[innerIndex];
                studentList[innerIndex] = studentList[innerIndex + 1];

                studentList[innerIndex + 1] = tempStudent;
            }
        }
    }
}

double find_Total_Marks(double mark1, double mark2, double mark3){
    return mark1 + mark2 + mark3;
}

double find_Average_Marks(double totalMarks)
{
    return totalMarks / 3.0;
}

char find_Grade(double averageMarks)
{
    if (averageMarks >= 85 && averageMarks <= 100)
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

void find_Performance(char grade)
{
    if (grade == 'A')
        printf("Performance: *****\n");
    else if (grade == 'B')
        printf("Performance: ****\n");
    else if (grade == 'C')
        printf("Performance: ***\n");
    else if (grade == 'D')
        printf("Performance: **\n");

    printf("\n");
}

void displayAllStudentDetails(Student studentList[], int totalStudents)
{
    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++)
    {
        printf("\n-------------------\n");
        printf("Roll number : %d\n", studentList[studentIndex].rollNumber);
        printf("Name  : %s\n", studentList[studentIndex].studentName);
        printf("Total marks : %.2lf\n", studentList[studentIndex].totalMarks);

        printf("Average marks : %.2lf\n", studentList[studentIndex].averageMarks);
        printf("Grade   : %c\n", studentList[studentIndex].grade);

        if (studentList[studentIndex].averageMarks >= 35)
            find_Performance(studentList[studentIndex].grade);
    }
}

int main()
{
    char inputBuffer[100];
    int totalStudents;

    printf("enter the number of students  : ");
    scanf("%s", inputBuffer);

    if (!isValidIntegerInput(inputBuffer))
    {
        printf("error: invalid input, please enter a valid integer for the number of students \n");
        return 1;
    }

    totalStudents = atoi(inputBuffer);

    if (totalStudents < 1 || totalStudents > 100)
    {
        printf("error: number of students must be between  [  1 and 100] \n");
        return 1;
    }

    Student studentList[100];
    char rollInputBuffer[50];

    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++)
    {
        printf("\n enter roll_number, Name, Marks1, Marks2, Marks3 for student %d: ", studentIndex + 1);

        if (scanf("%s %49s %lf %lf %lf", rollInputBuffer, studentList[studentIndex].studentName,
                  &studentList[studentIndex].subjectMark1,
                  &studentList[studentIndex].subjectMark2,
                  &studentList[studentIndex].subjectMark3) != 5)
        {
            printf("error: invalid input format, please enter: roll_Number Name Marks1 marks2 marks3 \n");
            return 1;
        }

        if (!isValidIntegerInput(rollInputBuffer))
        {
            printf("error: roll number must be a positive integer\n");
            return 1;
        }

        studentList[studentIndex].rollNumber = atoi(rollInputBuffer);

        if (studentList[studentIndex].rollNumber <= 0 || studentList[studentIndex].rollNumber > 99999)
        {
            printf("error: roll number must be between (1 and 99999)\n");
            return 1;
        }

        if (studentList[studentIndex].subjectMark1 < 0 || studentList[studentIndex].subjectMark1 > 100 ||
            studentList[studentIndex].subjectMark2 < 0 || studentList[studentIndex].subjectMark2 > 100 ||
            studentList[studentIndex].subjectMark3 < 0 || studentList[studentIndex].subjectMark3 > 100)
        {
            printf("error: marks must be between (0 and 100) \n");
            return 1;
        }

        studentList[studentIndex].totalMarks = find_Total_Marks(
            studentList[studentIndex].subjectMark1,
            studentList[studentIndex].subjectMark2,
            studentList[studentIndex].subjectMark3);

        studentList[studentIndex].averageMarks = find_Average_Marks(studentList[studentIndex].totalMarks);
        studentList[studentIndex].grade = find_Grade(studentList[studentIndex].averageMarks);
    }

    sortStudents_ByRollNumber(studentList, totalStudents);
    displayAllStudentDetails(studentList, totalStudents);

    printf("\nlist of Roll Numbers (via recursion): ");
    printRollNumbers(studentList, 0, totalStudents);
    printf("\n\n");

    return 0;
}
