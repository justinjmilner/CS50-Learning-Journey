#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int firstNumberV;
    int firstNumberMC;
    int firstNumberAM;
    int lengthV;
    int lengthMC;
    int lengthAM;
    int luhnAlgo;

    //Ask for credit card number
    long credit = get_long("Enter credit card number here: ");
    return credit;

//Determine the card type by first number
long firstNumber = credit;
while(firstNumber >= 10)
{
    firstNumber / 10;
}
return firstNumber;
int fn = firstNumber;

if (fn == 4)
{
    firstNumberV = 1;
}

if (fn == (34 | 37))
{
    firstNumberMC = 1;
}

if (fn == (51 | 52 | 53 | 54 | 55))
{
    firstNumberAM = 1;
}

//Determine the length of the card number
long length = credit;
int l=1;
while(length>9){ length/=10; l++; }
return l;

if (l == (13 | 16))
{
    lengthV = 1;
}

if (l == 16)
{
    lengthMC = 1;
}

if (l == 15)
{
    lengthAM = 1;
}

//Calculate Luhns Algorithm
int n;
long luhnAlgo1 = credit;
long luhnAlgo2 = credit / 10;
int sum1;
long sum2;

do
{
    sum1 += luhnAlgo1%10;
    luhnAlgo1/100;
}
while(n = 0, n < 8, n++);
return sum1;

//Returns 2nd to last number, every second number thereafter...
int modulo;
do
{
    modulo = luhnAlgo2%10;
    sum2 += modulo*2;
    luhnAlgo2/100;
}
while(n = 0, n < 8, n++);
return sum2;

int sum3 = sum1 + sum2;

if (sum3%10 == 0)
{
    luhnAlgo = 1;
}

//Return card length + first number + algorithm result
if (firstNumberV == 1 & lengthV == 1 & luhnAlgo == 1)
{
    printf("VISA\n");
    return 0;
}
else
{
    printf("INVALID\n");
    return 0;
}

if (firstNumberMC == 1 & lengthMC == 1 & luhnAlgo == 1)
{
    printf("MASTERCARD\n");
    return 0;
}
else
{
    printf("INVALID\n");
    return 0;
}

if (firstNumberAM == 1 & lengthAM == 1 & luhnAlgo == 1)
{
    printf("AMEX\n");
    return 0;
}
else
{
    printf("INVALID\n");
    return 0;
}

}