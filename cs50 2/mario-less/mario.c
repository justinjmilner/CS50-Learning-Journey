#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Declare the variable and type
    int n;
    //Ask for height only between 1-8
    do
    {
        n = get_int("Height: ");
    }
    while (n > 8 || n < 1);
    //Repeat a new line as many times as height
    for (int i = 0; i < n; i++)
    {
        //Prints the pyramid width
        for (int k = (n - 1); k > i; k--)
        {
            printf(" ");
        }
        for (int j = -1; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}