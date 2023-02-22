#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    //only allow 1 argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    //argument must be digits
    bool digits = only_digits(argv[1]);
    if (digits == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    //convert argument string to int
    int key = atoi(argv[1]);

    string text = get_string("Plaintext: ");
    //pass elements in the string array through rotate
    printf("Ciphertext: ");
    for (int m = 0, o = strlen(text); m < o; m++)
    {
        char z = rotate(text[m], key);
        printf("%c", z); //prints rotated char's as they come
    }
    printf("\n");
    return 0;
}
//checks that string is only digits
bool only_digits(string s)
{
    bool l = false;
    int k = 0;
    for (int i = 0, j = strlen(s); i < j; i++)
    {
        if (isdigit(s[i]))
        {
            k++;
        }
        if (k == j)
        {
            l = true;
        }
    }
    return l;
}
//rotates chars by key value
char rotate(char c, int n)
{
    if (isupper(c))
    {
        c -= 65;
        c += n;
        c = c % 26;
        c += 65;
    }
    else if (islower(c))
    {
        c -= 97;
        c += n;
        c = c % 26;
        c += 97;
    }
    return c;
}