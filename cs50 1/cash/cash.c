#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

// Ask how many cents the customer is owed
int get_cents(void)
{
    int cents;
    do
    {
        cents = get_int("Amount of change: ");
    }
    while (cents < 0);
    return cents;
}
// Calculate the number of quarters to give the customer
int calculate_quarters(int cents)
{
    int quarters;
    quarters = cents / 25;
    return quarters;
}
// Calculate the number of dimes to give the customer
int calculate_dimes(int cents)
{
    int dimes;
    // cents = cents % 25;
    dimes = cents / 10;
    return dimes;
}
// Calculate the number of nickels to give the customer
int calculate_nickels(int cents)
{
    int nickels;
    // cents = cents % 10;
    nickels = cents / 5;
    return nickels;
}
// Calculate the number of pennies to give the customer
int calculate_pennies(int cents)
{
    int pennies;
    // cents = cents % 5;
    pennies = cents / 1;
    return pennies;
}