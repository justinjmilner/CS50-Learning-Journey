#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Asks for a height between 1-20
    int n;
    do
    {
        n = get_int("Input pyramid brick height: ");
         if (n > 20 || n < 1)
    { printf("Try inputting another number!\n"); }
    }
    while (n > 20 || n < 1);


    //Prints the # pyramid
    for (int i = 0; i < n; i++)
    {
        for (int k = (n - 1); k > i; k--)
        {
            printf(" ");
        }
        for (int j = -1; j < i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int l = -1; l < i; l++)
        {
            printf("#");
        }

        printf("\n");
    }
}

