# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 10000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    # Create a list called teams
    teams = []

    # Store the data file in a variable
    filename = sys.argv[1]
    # Open the file as variable f
    with open(filename) as f:
        # Store each row from the file as a dictionary in the reader object
        # This will look like [{team: "Portugal", rating: "1500"}, {...}, {...}]
        # Each dictionary has 2 key value pairs. One for the name, one for the value.
        reader = csv.DictReader(f)
        # Iterate through each team in reader
        for team in reader:
            # Change each team rating to an int
            team["rating"] = int(team["rating"])
            # Append the team to the list called teams
            teams.append(team)

    # Create an empty dictionary to hold one key value pair
    # key = team name(winner): value = how many wins
    counts = {}
    # Simulate N tournaments
    for i in range(N):
        # Store the winning team name in variable winner
        winner = simulate_tournament(teams)
        # Add one win if the winner already exists in the dictionary
        if winner in counts:
            counts[winner] += 1
        # Add the team to the dictionary if they don't yet exist inside
        # And set the value of the team to one
        else:
            counts[winner] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    while len(teams) > 1:
        # Keep simulating rounds until teams is just 1 remaining dictionary
        teams = simulate_round(teams)
    # return the first (and only) dictionary in teams
    # and just the team column to return only the name
    return teams[0]["team"]


if __name__ == "__main__":
    main()
