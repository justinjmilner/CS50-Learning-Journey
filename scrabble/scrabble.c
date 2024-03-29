#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    printf("Player 1 score %i\n", score1);
    printf("Player 2 score %i\n", score2);
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("It's a tie!");
    }
    printf("\n");
    return 0;
}

int compute_score(string word)
{
    // Compute and return score for string
    int v = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isupper(word[i]))
        {
            int t = word[i] - 65;
            v += POINTS[t];
        }
        if (islower(word[i]))
        {
            int t = word[i] - 97;
            v += POINTS[t];
        }
    }
    return v;
}
