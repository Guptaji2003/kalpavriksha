#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int findprecedence(char operator)
{
    if (operator == '+' || operator == '-')
        return 1;
    if (operator == '*' || operator == '/')
        return 2;
    return 0;
}

int performOperation(int val1, int val2, char operator)
{
    if (operator == '+')
        return val1 + val2;
    if (operator == '-')
        return val1 - val2;
    if (operator == '*')
        return val1 * val2;
    if (operator == '/')
    {
        if (val2 == 0)
        {
            printf("error: division by 0\n");
            return 0;
        }
        return val1 / val2;
    }
    else
    {
        printf("error: invalid operator '%c'\n", operator);
        return 0;
    }
}

int evaluate(char expression[])
{
    int valuestack[MAX];
    int valuetop = -1;
    char operatorstack[MAX];
    int operatortop = -1;

    int expressionlength = strlen(expression);
    for (int i = 0; i < expressionlength; i++)
    {
        if (expression[i] == ' ')
            continue;

        if (isdigit(expression[i]))
        {
            int n = 0;
            while (i < expressionlength && isdigit(expression[i]))
            {
                n = n * 10 + (expression[i] - '0');
                i++;
            }
            valuestack[++valuetop] = n;
            i--;
        }
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
        {
            while (operatortop >= 0 && findprecedence(operatorstack[operatortop]) >= findprecedence(expression[i]))
            {
                int val2 = valuestack[valuetop--];
                int val1 = valuestack[valuetop--];
                char operator = operatorstack[operatortop--];

                int ans = performOperation(val1, val2, operator);
                valuestack[++valuetop] = ans;
            }
            operatorstack[++operatortop] = expression[i];
        }
        else
        {
            printf("error: invalid character '%c'\n", expression[i]);
            return 0;
        }
    }

    while (operatortop >= 0)
    {
        int val2 = valuestack[valuetop--];
        int val1 = valuestack[valuetop--];
        char operator = operatorstack[operatortop--];
        int ans = performOperation(val1, val2, operator);
        valuestack[++valuetop] = ans;
    }
    return valuestack[valuetop];
}

int main()
{
    char expression[MAX];
    printf("enter expression: ");
    fgets(expression, MAX, stdin);
    expression[strcspn(expression, "\n")] = 0;
    int result = evaluate(expression);
    printf("Result: %d\n", result);
    return 0;
}
