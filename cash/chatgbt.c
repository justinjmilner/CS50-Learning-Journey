#include <stdio.h>

int main() {
  // Initialize variables
  int change = 0;
  int coins = 0;
  int quarters = 0;
  int dimes = 0;
  int nickels = 0;
  int pennies = 0;

  // Prompt the user to enter the amount of change
  printf("Enter the amount of change: ");
  scanf("%d", &change);

  // Calculate the minimum number of coins
  quarters = change / 25;
  change %= 25;
  dimes = change / 10;
  change %= 10;
  nickels = change / 5;
  change %= 5;
  pennies = change;
  coins = quarters + dimes + nickels + pennies;

  // Print the result
  printf("The minimum number of coins is: %d\n", coins);

  return 0;
}
