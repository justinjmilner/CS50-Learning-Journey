import re


def main():
    text = get_text()
    letters = letter_count(text)
    words = (words_count(text)) + 1
    sentences = sentences_count(text)
    grade = grade_level(text, letters, words, sentences)
    print(grade)


# Ask user to input text
def get_text():
    text = input("Input text: ")
    return text


# Count number of alphabetical chars in the text
def letter_count(text):
    return len(re.findall('[a-zA-Z]', text))


# Count number of words in the text
def words_count(text):
    return len(re.findall(' ', text))


# Count numbe of sentences in the text
def sentences_count(text):
    return len(re.findall('[\?\.\!]', text))


# Calculate grade level from Coleman-Liau index
def grade_level(text, letters, words, sentences):
    L = letters / (words / 100)
    S = sentences / (words / 100)
    grade = round(0.0588 * L - 0.296 * S - 15.8)
    if grade < 1:
        return "Before Grade 1"
    elif grade > 16:
        return "Grade 16+"
    else:
        return f"Grade {grade}"


if __name__ == "__main__":
    main()