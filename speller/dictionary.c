// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Keeping track of how many words have been hashed from the dictionary
int words = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table is the sqrt of total dictionary words
const unsigned int N = 378;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Obtain hash value of the word and create temporary pointer to parse the linked list
    int hashValue = hash(word);
    node *tmp = table[hashValue];

    // Check if that bucket in the hash table contains the word
    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp -> word) == 0)
        {
            return true;
        }
        tmp = tmp -> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int totalValue = 0;
    unsigned int n = strlen(word);
    char wordUpper[LENGTH + 1];

    // Convert the word to all caps
    for (int i = 0; i < n; i++)
    {
        wordUpper[i] = toupper(word[i]);
    }

    // Get the total ASCII value of chars in the word
    for (int i = 0; i < strlen(word); i++)
    {
        totalValue += wordUpper[i];
    }

    // Get the hash table index value
    return totalValue % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary and check file loaded
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Scan a word from the dictionary into a buffer array
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a node for the word; check if malloc is available
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        // Copy the word from the buffer array, into the node
        strcpy(n -> word, word);

        // Hash the word
        int hashValue = hash(word);

        // Append word into linked list
        n -> next = table[hashValue];
        table[hashValue] = n;

        // Keep track of how many words pass through
        words++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through each of the buckets in the hash table
    for (int i = 0; i < N; i++)
    {
        node *tmp1 = table[i];
        while (tmp1 != NULL)
        {
            node *tmp2 = tmp1;
            tmp1 = tmp1->next;
            free(tmp2);
        }
        free(tmp1);
    }
    return true;
}