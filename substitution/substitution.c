#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool check_alpha(string s);
char rotate(char t, string u);
bool duplicates(string r);

int main(int argc, string argv[])
{
    //must input 1 argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //key length must be 26
    int l = strlen(argv[1]);
    if (l != 26)
    {
        printf("Must be 26 letters\n");
        return 1;
    }
    //key must be alphabetical
    bool alpha = check_alpha(argv[1]);
    if (alpha == false)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //key must not duplicate letters
    bool duplicated = duplicates(argv[1]);
    if (duplicated == true)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string text = get_string("plaintext: ");
    //encrypt plaintext
    printf("ciphertext: ");
    for (int x = 0, y = strlen(text); x < y; x++)
    {
        char z = rotate(text[x], argv[1]);
        printf("%c", z);
    }
    printf("\n");
    return 0;
}
//check if plaintext is alphabetical
bool check_alpha(string s)
{
    int a = 0;
    bool alpha = false;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isalpha(s[i]))
        {
            a++;
        }
        if (a == n)
        {
            alpha = true;
        }
    }
    return alpha;
}
//check for duplicate letters in key
bool duplicates(string r)
{
    bool duplicate = false;
    int d = 0;
    //compare value of each letter against each other
    for (int q = 0, w = strlen(r); q < w; q++)
    {
        for (int x = q, y = strlen(r); x < y; x++)
        {
            if (toupper(r[q]) != toupper(r[x]))
            {
                d++;
            }
        }
    }
    //add the total number of checks - 1 for each letter comparing to itself
    if (d != (strlen(r) * strlen(r) - strlen(r)))
    {
        duplicate = true;
    }
    return duplicate;
}
//rotate plaintext by key
char rotate(char t, string u)
{
    char c = t;
    int k = 0;
    //convert char to alphabetical position
    if (isupper(t))
    {
        k = t - 65;
        //swap position in alphabet with position in string
        if (isupper(u[k]))
        {
            c = u[k];
        }
        if (islower(u[k]))
        {
            c = u[k] - 32;
        }
    }
    if (islower(t))
    {
        k = t - 97;
        if (isupper(u[k]))  //swap position in alphabet with position in string
        {
            c = u[k] + 32;

        }
        if (islower(u[k]))
        {
            c = u[k];
        }

    }
    return c;
}
