#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }
    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    bool validity = false;
    for (int i = 0; i < candidate_count; i++) // go through every name added to the array
    {
        if (strcmp(name, candidates[i].name) == 0)  // compare voted name vs candidate array
        {
            candidates[i].votes++;
            validity = true;
        }
    }
    return validity;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int n = candidate_count;
    for (int i = 0; i < n; i++) // go through each candidate
    {
        for (int j = 0; j < (n - 1); j++) // go through each candidate again minus the last one
        {
            if (candidates[j].votes > candidates[j + 1].votes) // move higher values to the end of the array
            {
                int temp = candidates[j].votes;            // execute the swap
                candidates[j].votes = candidates[j + 1].votes;
                candidates[j + 1].votes = temp;
                string temp2 = candidates[j].name;            // execute the swap
                candidates[j].name = candidates[j + 1].name;
                candidates[j + 1].name = temp2;
            }
        }
    }
    for (int k = 0; k < n; k++)
    {
        if (candidates[k].votes == candidates[n - 1].votes)   // print the highest number in the candidate array
        {
            printf("%s\n", candidates[k].name);
        }
    }
    return;
}
