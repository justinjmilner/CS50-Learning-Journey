from cs50 import get_string
import re

# Create a dictionary with card name and the corresponding pattern
credit_cards = {"AMEX" : "3[47][0-9]{13}",
                "MASTERCARD" : "5[12345][0-9]{14}",
                "VISA" : "4[0-9]{12,15}"}


# Check if card passes all tests
def main():
    result = "INVALID"
    number = getInput()
    if luhnsAlgo(number) == 0:
        for i in credit_cards.keys():
            if re.match(credit_cards[i], number):
                result = type
    print(f"{result}")


# Get valid user to input credit card number
def getInput():
    while True:
        number = get_string("Input credit card number: ")
        if int(number) > 0:
            return number


# Caculate value of Luhns Algo
def luhnsAlgo(number):
    digits = digits_of(number)
    odd_digits = digits[-1::-2]
    even_digits = digits[-2::-2]
    checksum = 0
    checksum += sum(odd_digits)
    for d in even_digits:
        checksum += sum(digits_of(d*2))
    return checksum % 10


# Convert input to a list of single digit numbers
def digits_of(s):
    return [int(d) for d in str(s)]


if __name__ == "__main__":
    main()