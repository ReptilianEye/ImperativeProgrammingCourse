#include <stdio.h>

int main() {
    printf("Podaj liczbę elementow ciagu Fibonacciego\n ");
    int n;
    scanf("%d10",&n);
    if (n < 0) {
        printf("Liczba powinna być większa od 0");
    }
    else if (n==0)
        printf("\n");
    else if (n==1)
        printf("%d\n",1);
    else{
        long int a=1,b=1;
//        a=b=1;
        while (n>0)
        {
            printf("%d\n",b);
            a=a+b;
            b=a-b;
            n--;
        }
    }
    return 0;


}
