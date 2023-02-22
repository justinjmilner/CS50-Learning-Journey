import cs50
while True:
    height = cs50.get_int("Input pyramid height: ")
    if  0 < height < 9:
        break


for i in range(1, height + 1):
    print(" " * (height - i) + "#" * i)