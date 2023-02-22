#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{

// Get card number
    long cc_number = get_long("Enter credit card number here: ");

// Get length
    long length = cc_number;
    int i = 0;
    while (length > 0)
    {
        length = length / 10;
        i++;
    }

// Check if length is valid

    if (i != 13 && i != 15 && i != 16)
    {
        printf("INVALID\n");
        return 0;
    }

// Calculate checksum
    long n = cc_number;
    int mod1 = 0;
    int sum1 = 0;
    int sum2 = 0;
    int mod2 = 0;
    int add1 = 0;
    int add2 = 0;
    int total = 0;
    int mult = 0;
    do
    {
        // Get the last number
        mod1 = n % 10;
        // Add last number
        sum1 += mod1;
        // Get second last number
        n = n / 10;
        mod2 = n % 10;
        // Multiply that by 2
        mult = mod2 * 2;
        // Add one, or both, digits
        add1 = mult % 10;
        add2 = mult / 10;
        sum2 += (add1 + add2);
        n = n / 10;
    }
// Repeat until cc number hits 0
    while (n > 0);

    total = sum1 + sum2;

// Check Luhns Algo
    if (total % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

// Get first 2 digits
    long first2 = cc_number;
    do
    {
        first2 = first2 / 10;
    }
    while (first2 >= 100);

// Check starting digits
// Check for mastercard
    if ((first2 / 10 == 5) && (0 < first2 % 10 && first2 % 10 < 6))
    {
        printf("MASTERCARD\n");
    }
    else if ((first2 / 10 == 3) && (first2 % 10 == 4 || first2 % 10 == 7))
    {
        printf("AMEX\n");
    }
    else if (first2 / 10 == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}
