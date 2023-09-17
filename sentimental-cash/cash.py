import cs50
import numpy as np


while True:
    changeInit = cs50.get_float("Change in dollars: ")
    if changeInit > 0:
        break


change = changeInit
quarters = 0
dimes = 0
nickels = 0
pennies = 0


for i in np.arange (0.25, change + 0.01, .25):
    quarters += 1
    change -= .25


for i in np.arange (0.1, change + 0.01, .10):
    dimes += 1
    change -= .1


for i in np.arange (0.05, change + 0.01, .05):
    nickels += 1
    change -= .05


for i in np.arange (0.01, change + 0.001, .01):
    pennies += 1


coins = quarters + dimes + nickels + pennies


print(f"{coins}")

