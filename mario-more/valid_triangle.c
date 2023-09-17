#include <stdio.h>
#include <cs50.h>

bool valid_triangle(int a, int b, int c);

int main (void)
{
    int a = get_int("side 1: ");
    int b = get_int("side 2: ");
    int c = get_int("side 3: ");
    int triangle_validity = valid_triangle(a, b, c);
    printf("Triangle validity is %i. (Note: 0 is invalid, 1 is valid)\n", triangle_validity);
}

 bool valid_triangle(int a, int b, int c)
{
        if ( a <= 0 ||  b <= 0 ||  c <= 0)
        {
            return false;
        }

        if ((a >= c + b) || (b >= a + c) || (c >= a + b))
        {
            return false;
        }
        return true;
}

