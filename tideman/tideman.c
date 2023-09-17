#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool graph_cycle(int winner, int loser);
int pair_weight(int i);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++) // go through each candidate
    {
        if (strcmp(candidates[i], name) == 0) // find the correct voted name
        {
            ranks[rank] = i; // set the rank equal to the corresponding candidate
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1; // every time a candidate is preffered over another, add one to their count
        }   // didnt need to set the array to 0** // the count keeps track of how many times they were prefferd over others
    }                                           // the candidate who has the highest value was preffered the most over others
    return;
}


// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count - 1; i++) // compare every candidate
    {
        for (int j = i + 1; j < candidate_count; j++) // to every other candidate
        {
            if (preferences[i][j] > preferences[j][i]) // only allow pairs with differing values
            {
                pairs[pair_count].winner = i; // store the winner and loser of the pair
                pairs[pair_count].loser = j; // as the "nth" element in the pairs array
                pair_count ++; // update the pair count to go to the next element in the array
            }
            if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count ++;
            }
        }
    }
    return;
}

int pair_weight(i)
{
    int pair_winner = pairs[i].winner;
    int pair_loser = pairs[i].loser;
    return preferences[pair_winner][pair_loser]; // return how many times this winner is preferred over this loser
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = pair_count - 1; i >= 0; i--) // compare each pair starting at the end
    {
        int min_weight = pair_weight(i); // calculate the difference between the winner and loser
        int min_idx = i;
        for (int j = i - 1; j >= 0; j--)
        {
            if (pair_weight(j) < min_weight)
            {
                min_weight = pair_weight(j);
                min_idx = j;
            }
        }
    pair temp = pairs[min_idx];
    pairs[min_idx] = pairs[i];
    pairs[i] = temp;
    }
    return;
}

// Check for a cycle in the Tideman graph
bool graph_cycle(winner, loser)
{
    if (locked[loser][winner] == true) // check if the losing candidate points back at the winning candidate
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++) // compare the loser against the next candidate
    {
        if (locked[loser][i] == true && graph_cycle(winner, i)) // if the loser is locked in against someone else
        {                                                // that the winner is about to get locked into
            return true;                                // then thats about to create a cycle
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner; // store the winning candidate for the pair here
        int loser = pairs[i].loser; // store the losing candidate for the pair here
        if (!graph_cycle(winner, loser)) // check if locking the pair creates a cycle
        {
            locked[winner][loser] = true; // otherwise, lock the pair in to the graph
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void) // TODO understand
{
    int x;
    for (int i = 0; i < candidate_count; i++) // compare every candidate
    {
        x = 1;
        for (int j = 0; j < candidate_count; j++) // to every candidate
        {
            if (locked[j][i] == true) // if anyone dominates the candidate, they arent the winner
            {
                x = 0;
            }
        }
        if (x == 1) // if there are no arrows pointing back, break the loop and print winner
        {
            printf("%s\n", candidates[i]); // this candidate wins
            break;
        }
    }
    return;
}