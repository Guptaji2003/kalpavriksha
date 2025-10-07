#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int findprecedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

int performOperation(int a, int b, char op)
{
    if (op == '+')
        return a + b;
    if (op == '-')
        return a - b;
    if (op == '*')
        return a * b;
    if (op == '/')
    {
        if (b == 0)
        {
            printf("error: division by 0\n");
            return 0;
        }
        return a / b;
    }
    else
    {
        printf("error: invalid operator '%c'\n", op);
        return 0;
    }
}

int evaluate(char s[])
{
    int valuestack[MAX];
    int valuetop = -1;
    char operatorstack[MAX];
    int operatortop = -1;

    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        if (s[i] == ' ')
            continue;

        if (isdigit(s[i]))
        {
            int n = 0;
            while (i < len && isdigit(s[i]))
            {
                n = n * 10 + (s[i] - '0');
                i++;
            }
            valuestack[++valuetop] = n;
            i--;
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
        {
            while (operatortop >= 0 && findprecedence(operatorstack[operatortop]) >= findprecedence(s[i]))
            {
                int b = valuestack[valuetop--];
                int a = valuestack[valuetop--];
                char c = operatorstack[operatortop--];

                int ans = performOperation(a, b, c);
                valuestack[++valuetop] = ans;
            }
            operatorstack[++operatortop] = s[i];
        }
        else
        {
            printf("error: invalid character '%c'\n", s[i]);
            return 0;
        }
    }

    while (operatortop >= 0)
    {
        int b = valuestack[valuetop--];
        int a = valuestack[valuetop--];
        char c = operatorstack[operatortop--];
        int ans = performOperation(a, b, c);
        valuestack[++valuetop] = ans;
    }
    return valuestack[valuetop];
}

int main()
{
    char exp[MAX];
    printf("enter expression: ");
    fgets(exp, MAX, stdin);
    exp[strcspn(exp, "\n")] = 0;
    int result = evaluate(exp);
    printf("Result: %d\n", result);
    return 0;
}
