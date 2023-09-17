#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    bool voteResult = false;
    for (int i = 0; i < candidate_count; i++) // Sort through each candidate
    {
        if ((strcmp(candidates[i].name, name) == 0) && candidates[i].eliminated == false) // Check if the voted name exists
        {
            voteResult = true;
            preferences[voter][rank] = i;  // connect the candidate number with the voters preference rank
        }
    }
    return voteResult;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void) // TODO
{
    for (int i = 0; i < voter_count; i++) // go through each voter
    {
        for (int j = 0; j < candidate_count; j++) // go through each candidate per the voter
        {
            if (candidates[preferences[i][j]].eliminated == false)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    bool winner = false;
    int majorityVotes = voter_count / 2;
    for (int i = 0; i < candidate_count; i++) // go through each candidate
        {
            if (candidates[i].votes > majorityVotes) // check for majority of votes
            {
                winner = true;
                printf("%s\n", candidates[i].name);
            }
        }
    return winner;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int minVotes = voter_count;
        for (int i = 0; i < candidate_count; i++) // go through each candidate
        {
            if (candidates[i].votes < minVotes && candidates[i].eliminated == false) // checking for the smallest vote count
            {
                minVotes = candidates[i].votes; // and return the minimum value
            }
        }
    return minVotes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    bool tie = true;
    for (int i = 0; i < candidate_count; i++) // go through each candidate
    {
         for (int j = 0; j < candidate_count; j++) // compare each candidate to each candidate
         {
            if (candidates[i].votes != candidates[j].votes && candidates[i].eliminated == false && candidates[j].eliminated == false)
            {
                tie = false;
            }
         }
    }
    return tie;
}

// Eliminate the candidate (or candidates) in last place // TODO update the candidate to the
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++) // go through each candidate
    {
        if (candidates[i].votes == min) // if the candidate has the minimum value of votes
        {
            candidates[i].eliminated = true; // set candidate to eliminated
        }
    }
    return;
}
