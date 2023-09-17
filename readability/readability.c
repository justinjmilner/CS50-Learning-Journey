#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

//Declare global functions
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int determine_grade(int letters, int words, int sentences);

int main(void)
{
    //Ask for text
    string text = get_string("Text: ");
    //Get letter count
    int letters = count_letters(text);
    //Get word count
    int words = count_words(text);
    //Get sentence count
    int sentences = count_sentences(text);
    //Get grade level
    int grade = determine_grade(letters, words, sentences);
    //Print grade level
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
    return 0;
}
//Calculate letters
int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}
//Calculate words
int count_words(string text)
{
    int words = 0;
    for (int i = 0, n = strlen(text); i <= n; i++)
    {
        if (isspace(text[i]) || text[i] == 0)
        {
            words++;
        }
    }
    return words;
}
//Calculate sentences
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            sentences++;
        }
    }
    return sentences;
}
//Calculate grade
int determine_grade(letters, words, sentences)
{
    float fletters = letters;
    float fwords = words;
    float fsentences = sentences;
    float L = ((fletters / fwords) * 100.0);
    float S = ((fsentences / fwords) * 100.0);
    float index = ((0.0588 * L) - (0.296 * S) - 15.8);
    int grade = round(index);
    return grade;
}
