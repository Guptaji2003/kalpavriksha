#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int pri(char c) {
    if(c=='+' ||c=='-')  return 1;
    if( c=='*'|| c== '/') return 2;
    return 0;
}

int calc(int a,int b,char c) {
    if( c=='+') return a+b;
    if(c=='-') return a-b;
    if(c=='*') return a*b;
    if(c=='/') {
        if(b==0) {
            printf("error- divide by 0\n");
            exit(0);
        }
        return a/b;
    }
    return 0;
}

int eval(char s[]) {
    int val[100];
    int vt=-1;
    char op[100]; 
    int ot=-1;

    for(int i=0; i<strlen(s);i++) {
        if(s[i]==' ') continue;

        if(isdigit(s[i])) {
            int n=0;
            while(i<strlen(s)&&isdigit(s[i])) {
                n=n*10+(s[i]-'0');
                i++;
            }

            val[++vt] = n;
            i--;
        } else if(s[i]=='+'|| s[i]=='-'|| s[i]=='*'||s[i]=='/') {
            while(ot>=0 && pri(op[ot])>=pri(s[i])) {
                int b=val[vt--];
                int a=val[vt--];
                char c=op[ot--];
                
                val[++vt]=calc(a,b,c);
            }
            op[++ot]=s[i];
        } else {
            printf("error: invalid expression\n");
            exit(0);
        }
    }
    while(ot>=0) {
        int b=val[vt--];
        int a =val[vt--];
        char c =op[ot--];

        val[++vt] = calc(a,b,c);
    }
    return val[vt];
}

int main() {
    char exp[200];
    printf("enter your expression : ");
    fgets(exp,200,stdin);
    exp[strcspn(exp,"\n")]=0;
    int ans=eval(exp);
    printf("ans is %d\n",ans);
    return 0;
}
